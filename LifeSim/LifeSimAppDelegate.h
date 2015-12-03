//
//  LifeSimAppDelegate.h
//  LifeSim
//
//  Created by Cameron Upright on 12-04-04.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface LifeSimAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
