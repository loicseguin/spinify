//
//  SpinController.h
//  SpinWithController
//
//  Created by Loïc Séguin-Charbonneau on 10-06-10.
//  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface SpinController : NSObject {
    NSThread *spinThread;
    NSTask *spinify;
    IBOutlet NSArrayController *arrayController;
    IBOutlet NSButton *runButton;
    IBOutlet NSButton *stopButton;
}

- (IBAction)run:(id)sender;
- (IBAction)stop:(id)sender;
- (void)launchAndListen:(id)data;

@end
