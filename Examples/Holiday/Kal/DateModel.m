//
//  DateModel.m
//  Holiday
//
//  Created by allen on 13-6-20.
//
//

#import "DateModel.h"

@implementation DateModel {
@private
    NSString *_m_year;
    NSString *_m_month;
    NSString *_m_day;
}
@synthesize m_year = _m_year;
@synthesize m_month = _m_month;
@synthesize m_day = _m_day;

- (id)initWithM_year:(NSString *)aM_year m_month:(NSString *)aM_month m_day:(NSString *)aM_day {
    self = [super init];
    if (self) {
        self.m_year = aM_year;
        self.m_month = aM_month;
        self.m_day = aM_day;
    }

    return self;
}

- (void)dealloc {
    [_m_year release];
    [_m_month release];
    [_m_day release];
    [super dealloc];
}


- (NSString *)description {
    NSMutableString *description = [NSMutableString stringWithFormat:@"<%@: year: %@ month: %@ day: %@", NSStringFromClass([self class]),self.m_year,self.m_month,self.m_day];
    [description appendString:@">"];
    return description;
}


@end
