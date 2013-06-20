//
//  main.m
//  Holiday
//
//  Created by Keith Lazuka on 7/5/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    //直接忽略SIGPIPE信号，避免程序在收到Broken Pipe时退出
    signal(SIGPIPE, SIG_IGN);
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
