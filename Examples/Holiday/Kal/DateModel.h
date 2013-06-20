//
//  DateModel.h
//  Holiday
//
//  Created by allen on 13-6-20.
//
//

#import <Foundation/Foundation.h>

@interface DateModel : NSObject

@property(nonatomic, copy) NSString *m_year;
@property(nonatomic, copy) NSString *m_month;
@property(nonatomic, copy) NSString *m_day;

- (id)initWithM_year:(NSString *)aM_year m_month:(NSString *)aM_month m_day:(NSString *)aM_day;




@end
