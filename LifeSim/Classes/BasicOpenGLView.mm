//
// File:		BasicOpenGLView.m
//
// Abstract:	Basic OpenGL View with Renderer information
//
// Version:		1.1 - minor fixes.
//				1.0 - Original release.
//				
//
// Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Inc. ("Apple")
//				in consideration of your agreement to the following terms, and your use,
//				installation, modification or redistribution of this Apple software
//				constitutes acceptance of these terms.  If you do not agree with these
//				terms, please do not use, install, modify or redistribute this Apple
//				software.
//
//				In consideration of your agreement to abide by the following terms, and
//				subject to these terms, Apple grants you a personal, non - exclusive
//				license, under Apple's copyrights in this original Apple software ( the
//				"Apple Software" ), to use, reproduce, modify and redistribute the Apple
//				Software, with or without modifications, in source and / or binary forms;
//				provided that if you redistribute the Apple Software in its entirety and
//				without modifications, you must retain this notice and the following text
//				and disclaimers in all such redistributions of the Apple Software. Neither
//				the name, trademarks, service marks or logos of Apple Inc. may be used to
//				endorse or promote products derived from the Apple Software without specific
//				prior written permission from Apple.  Except as expressly stated in this
//				notice, no other rights or licenses, express or implied, are granted by
//				Apple herein, including but not limited to any patent rights that may be
//				infringed by your derivative works or by other works in which the Apple
//				Software may be incorporated.
//
//				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
//				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
//				WARRANTIES OF NON - INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
//				PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION
//				ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
//
//				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
//				CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//				SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//				INTERRUPTION ) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
//				AND / OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER
//				UNDER THEORY OF CONTRACT, TORT ( INCLUDING NEGLIGENCE ), STRICT LIABILITY OR
//				OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright ( C ) 2003-2007 Apple Inc. All Rights Reserved.
//

#import "BasicOpenGLView.h"

#import "g_axis.h"
#import "fpscounter.h"

#import "Scene.h"
#import "SceneVis.h"
#import "UserInputManager.h"

#include "timer.h"

Scene *scene;
SceneVis *sceneVis;

UserInputManager *userInputManager;

Timer simTimer;

FPScounter fps;

@implementation BasicOpenGLView

// pixel format definition
+ (NSOpenGLPixelFormat*) basicPixelFormat
{
	NSOpenGLPixelFormatAttribute attributes [] = {
		NSOpenGLPFAWindow,
		NSOpenGLPFADoubleBuffer,	// double buffered
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16, // 16 bit depth buffer
		NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute)8,
		(NSOpenGLPixelFormatAttribute)0
	};
	return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

// ---------------------------------

// update the projection matrix based on camera and view info
- (void) updateProjection
{

	[[self openGLContext] makeCurrentContext];

	// set projection
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

  gluPerspective(45.0f,(GLfloat)camera.viewWidth/(GLfloat)camera.viewHeight,0.1f,500.0f);	// Calculate The Aspect Ratio Of The Window

}

// ---------------------------------

// updates the contexts model view matrix for object and camera moves
- (void) updateModelView
{
	[[self openGLContext] makeCurrentContext];
	
	// move view
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glTranslated(0,0,-5);

}

// ---------------------------------

// handles resizing of GL need context update and if the window dimensions change, a
// a window dimension update, reseting of viewport and an update of the projection matrix
- (void) resizeGL
{
	NSRect rectView = [self bounds];
	
	// ensure camera knows size changed
	if ((camera.viewHeight != rectView.size.height) ||
	    (camera.viewWidth != rectView.size.width)) {
		camera.viewHeight = rectView.size.height;
		camera.viewWidth = rectView.size.width;
		
		glViewport (0, 0, camera.viewWidth, camera.viewHeight);
		[self updateProjection];  // update projection matrix
	}
}

#pragma mark ---- Animation Timer ----
- (void)animationTimer:(NSTimer *)timer
{

	CFTimeInterval deltaTime = CFAbsoluteTimeGetCurrent () - time;
                       
	if (deltaTime > 10.0) // skip pauses
		return;

	time = CFAbsoluteTimeGetCurrent (); //reset time in all cases

	[self drawRect:[self bounds]];
}



#pragma mark ---- IB Actions ----

-(IBAction) animate: (id) sender
{
}

// ---------------------------------

-(IBAction) info: (id) sender
{
	[self setNeedsDisplay: YES];
}

#pragma mark ---- Keyboard Input ----

-(void)keyDown:(NSEvent *)theEvent
{
	NSString *characters = [theEvent characters];
	if ([characters length]) {
		unichar character = [characters characterAtIndex:0];
		switch (character) {

		case 'h':
			[self setNeedsDisplay: YES];
			break;
		
		case 'q':
			[self.window performClose: self];
			break;

		}
	}
}

#pragma mark ---- Mouse Input ----

- (NSPoint) pointInViewCoordinates: (NSEvent*) theEvent {

	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	return NSMakePoint(location.x, self.bounds.size.height-location.y-1.0f);

}

- (void)mouseDown:(NSEvent *)theEvent {

	NSPoint location = [self pointInViewCoordinates: theEvent];
	bool shiftPressed = [theEvent modifierFlags] & NSShiftKeyMask;
	userInputManager->mouseDown(LeftButton, shiftPressed, (int)location.x, (int)location.y);

}

- (void) rightMouseDown:(NSEvent *)theEvent {

	NSPoint location = [self pointInViewCoordinates: theEvent];
	bool shiftPressed = [theEvent modifierFlags] & NSShiftKeyMask;
	userInputManager->mouseDown(RightButton, shiftPressed, (int)location.x, (int)location.y);

}

- (void) otherMouseDown:(NSEvent *)theEvent {

	NSPoint location = [self pointInViewCoordinates: theEvent];
	bool shiftPressed = [theEvent modifierFlags] & NSShiftKeyMask;
	userInputManager->mouseDown(MiddleButton, shiftPressed, (int)location.x, (int)location.y);

}


- (void)mouseUp:(NSEvent *)theEvent {

	NSPoint location = [self pointInViewCoordinates: theEvent];
	userInputManager->mouseUp((int)location.x, (int)location.y);

}

- (void) rightMouseUp:(NSEvent *)theEvent {

	[self mouseUp: theEvent];

}

- (void) otherMouseUp:(NSEvent *)theEvent {

	[self mouseUp: theEvent];

}



- (void)mouseDragged:(NSEvent *)theEvent {

	NSPoint location = [self pointInViewCoordinates: theEvent];
	userInputManager->mouseMotion((int)location.x, (int)location.y);

}

- (void) rightMouseDragged:(NSEvent *)theEvent {

	[self mouseDragged: theEvent];

}

- (void) otherMouseDragged:(NSEvent *)theEvent {

	[self mouseDragged: theEvent];

}


- (void)scrollWheel:(NSEvent *)theEvent {

	float wheelDelta = [theEvent deltaX] + [theEvent deltaY] + [theEvent deltaZ];

	userInputManager->scrollWheel(wheelDelta);

}



- (void) drawRect:(NSRect)rect
{		
	// setup viewport and prespective
	[self resizeGL]; // forces projection matrix update (does test for size changes)
	[self updateModelView];  // update model view matrix for object

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	simTimer.update();
	float elapsed = simTimer.get_dif_f();
	simTimer.reset();

	scene->step(elapsed);

	sceneVis->render();

	fps.showFrameRate();

	//	glFlush();
	[[self openGLContext] flushBuffer];

}

// ---------------------------------

// set initial OpenGL state (current context is set)
// called after context is created
- (void) prepareOpenGL
{
	GLint swapInt = 1;

	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; // set to vbl sync

	// init GL stuff here
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);    
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPolygonOffset (1.0f, 1.0f);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

}
// ---------------------------------

// this can be a troublesome call to do anything heavyweight, as it is called on window moves, resizes, and display config changes.  So be
// careful of doing too much here.
- (void) update // window resizes, moves and display changes (resize, depth and display config change)
{
	[super update];
}

// ---------------------------------

-(id) initWithFrame: (NSRect) frameRect
{
	NSOpenGLPixelFormat * pf = [BasicOpenGLView basicPixelFormat];

	self = [super initWithFrame: frameRect pixelFormat: pf];
	return self;
}

// ---------------------------------

- (BOOL)acceptsFirstResponder
{
  return YES;
}

// ---------------------------------

- (BOOL)becomeFirstResponder
{
  return  YES;
}

// ---------------------------------

- (BOOL)resignFirstResponder
{
  return YES;
}

// ---------------------------------

- (void) awakeFromNib
{
	time = CFAbsoluteTimeGetCurrent();

	timer = [NSTimer timerWithTimeInterval:(1.0f/60.0f) target:self selector:@selector(animationTimer:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode]; // ensure timer fires during resize

	scene = new Scene();
	sceneVis = new SceneVis(*scene);
	scene->setObserver(sceneVis);


	NSString *sceneFile = [[NSBundle mainBundle] pathForResource:@"scene" ofType:@"yaml" inDirectory: @"res"];

	scene->load([sceneFile cStringUsingEncoding:NSASCIIStringEncoding]);

	userInputManager = new UserInputManager(*sceneVis);

	simTimer.reset();

}


@end
