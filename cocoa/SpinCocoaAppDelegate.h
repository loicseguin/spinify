//
//  SpinCocoaAppDelegate.h
//  SpinCocoa
//
//  Created by Loïc Séguin-Charbonneau on 10-05-28.
//  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface SpinCocoaAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
    IBOutlet NSTableView *resultsTable;
    NSMutableArray *rows;
    NSThread *spinThread;
    IBOutlet NSButton *runButton;
    IBOutlet NSButton *stopButton;
}

@property (assign) IBOutlet NSWindow *window;
- (IBAction)run:(id)sender;
- (IBAction)stop:(id)sender;
- (int)numberOfRowsInTableView:(NSTableView *)aTableView;
- (id)tableView:(NSTableView *)aTableView
    objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)row;
- (void)launchAndListen:(id)data;

@end
