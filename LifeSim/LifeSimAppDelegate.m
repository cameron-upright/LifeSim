//
//  LifeSimAppDelegate.m
//  LifeSim
//
//  Created by Cameron Upright on 12-04-04.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "LifeSimAppDelegate.h"

@implementation LifeSimAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application 

	NSString *filePath = [[NSBundle mainBundle] pathForResource:@"asdf" ofType:@"txt" inDirectory:@"TestDir"];
	NSString* content = [NSString stringWithContentsOfFile:filePath
																								encoding:NSUTF8StringEncoding
																									 error:NULL];
	NSLog(@"%@", content);
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
	return YES;
}


@end
