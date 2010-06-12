//
//  SpinController.m
//  SpinWithController
//
//  Created by Loïc Séguin-Charbonneau on 10-06-10.
//  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
//

#import "SpinController.h"
#import "Measure.h"


@implementation SpinController

- (IBAction)run:(id)sender {    
    [runButton setEnabled:NO];
    [stopButton setEnabled:YES];
    
    [[arrayController content] removeAllObjects];
        
    spinThread = [[NSThread alloc] initWithTarget:self
                                         selector:@selector(launchAndListen:)
                                           object:nil];
    [spinThread start];
}

- (IBAction)stop:(id)sender {
    [spinThread cancel];
}

- (void)launchAndListen:(id)data {    
    spinify = [[NSTask alloc] init];
    [spinify setLaunchPath:@"/usr/local/bin/spinify"];
    [spinify setArguments:[NSArray arrayWithObjects:@"-s", @"100", @"-e",
                           @"-T", @"1", @"6", @"0.5", nil]];
    
    NSPipe *pipe =[NSPipe pipe];
    [spinify setStandardOutput:pipe];
    
    NSFileHandle *readHandle = [pipe fileHandleForReading];
    
    [spinify launch];
    
    NSData *inData = [[NSData alloc] init];
    while ((inData = [readHandle availableData]) && [inData length]) {
        if ([spinThread isCancelled]) {
            break;
        }
        NSString* aStr = [[NSString alloc] initWithData:inData
                                               encoding:NSASCIIStringEncoding];
        NSArray *tokens = [aStr componentsSeparatedByString:@" "];
        Measure *measure = [[Measure alloc]
                           initWithTemperature:[[tokens objectAtIndex:0]
                                                floatValue]
                                          data:[[tokens objectAtIndex:1]
                                                floatValue]];
        [arrayController addObject:measure];
        [measure release];
        [aStr release];
    }
    [inData release];
    [spinify release];
    [spinThread release];
    [runButton setEnabled:YES];
    [stopButton setEnabled:NO];
}

@end
