//
//  SpinCocoaAppDelegate.m
//  SpinCocoa
//
//  Created by Loïc Séguin-Charbonneau on 10-05-28.
//  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
//

#import "SpinCocoaAppDelegate.h"

@implementation SpinCocoaAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application
    [stopButton setEnabled:NO];
}

- (IBAction)run:(id)sender {
    rows = [NSMutableArray new];
    
    [runButton setEnabled:NO];
    [stopButton setEnabled:YES];
    //[self performSelectorInBackground:@selector(launchAndListen:) withObject:nil];
    spinThread = [[NSThread alloc] initWithTarget:self
                                         selector:@selector(launchAndListen:)
                                           object:nil];
    [spinThread start];
    
}

- (IBAction)stop:(id)sender {
    [spinThread cancel];
}

- (void)launchAndListen:(id)data {
    NSData *inData = nil;
    NSTask *spinify = [NSTask new];
    [spinify setLaunchPath:@"/usr/local/bin/spinify"];
    [spinify setArguments:[NSArray arrayWithObjects:@"-s", @"100", @"-e",
                           @"-T", @"1", @"6", @"0.5", nil]];
    NSPipe *pipe =[NSPipe pipe];
    [spinify setStandardOutput:pipe];
    NSFileHandle *readHandle = [pipe fileHandleForReading];
    [spinify launch];
    while ((inData = [readHandle availableData]) && [inData length]) {
        if ([spinThread isCancelled]) {
            break;
        }
        NSString* aStr;
        aStr = [[NSString alloc] initWithData:inData encoding:NSASCIIStringEncoding];
        NSArray *tokens = [aStr componentsSeparatedByString:@" "];
        [rows addObject:tokens];
        [resultsTable reloadData];
    }
    [spinify release];
    [spinThread release];
    [runButton setEnabled:YES];
    [stopButton setEnabled:NO];
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
    return [rows count];
}

- (id)tableView:(NSTableView *)aTableView
    objectValueForTableColumn:(NSTableColumn *)aTableColumn
            row:(int)row {
    if ([[aTableColumn identifier] isEqualToString:@"Temperature"]) {
        return [[rows objectAtIndex:row] objectAtIndex:0];
    }
    else {
        return [[rows objectAtIndex:row] objectAtIndex:1];
    }
}

    

@end
