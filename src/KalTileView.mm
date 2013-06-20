/*
 * Copyright (c) 2009 Keith Lazuka
 * License: http://www.opensource.org/licenses/mit-license.html
 */

#import "KalTileView.h"
#import "KalDate.h"
#import "IDJCalendarUtil.h"
#import "SolarDate.h"
#import "ChineseDate.h"
#import "KalPrivate.h"

extern const CGSize kTileSize;

@implementation KalTileView

@synthesize date;

- (id)initWithFrame:(CGRect)frame
{
    if ((self = [super initWithFrame:frame])) {
        self.opaque = NO;
        self.backgroundColor = [UIColor clearColor];
        self.clipsToBounds = NO;
        origin = frame.origin;
        [self setIsAccessibilityElement:YES];
        [self setAccessibilityTraits:UIAccessibilityTraitButton];
        [self resetState];
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGFloat fontSize = 24.f;
    UIFont *font = [UIFont boldSystemFontOfSize:fontSize];
    UIColor *shadowColor = nil;
    UIColor *textColor = nil;
    UIImage *markerImage = nil;
    CGContextSelectFont(ctx, [font.fontName cStringUsingEncoding:NSUTF8StringEncoding], fontSize, kCGEncodingMacRoman);
    
    CGContextTranslateCTM(ctx, 0, kTileSize.height);
    CGContextScaleCTM(ctx, 1, -1);
    
    if ([self isToday] && self.selected) {
        [[[UIImage imageNamed:@"Kal.bundle/kal_tile_today_selected.png"] stretchableImageWithLeftCapWidth:6 topCapHeight:0] drawInRect:CGRectMake(0, -1, kTileSize.width+1, kTileSize.height+1)];
        textColor = [UIColor whiteColor];
        shadowColor = [UIColor blackColor];
        markerImage = [UIImage imageNamed:@"Kal.bundle/kal_marker_today.png"];
    } else if ([self isToday] && !self.selected) {
        [[[UIImage imageNamed:@"Kal.bundle/kal_tile_today.png"] stretchableImageWithLeftCapWidth:6 topCapHeight:0] drawInRect:CGRectMake(0, -1, kTileSize.width+1, kTileSize.height+1)];
        textColor = [UIColor whiteColor];
        shadowColor = [UIColor blackColor];
        markerImage = [UIImage imageNamed:@"Kal.bundle/kal_marker_today.png"];
    } else if (self.selected) {
        [[[UIImage imageNamed:@"Kal.bundle/kal_tile_selected.png"] stretchableImageWithLeftCapWidth:1 topCapHeight:0] drawInRect:CGRectMake(0, -1, kTileSize.width+1, kTileSize.height+1)];
        textColor = [UIColor whiteColor];
        shadowColor = [UIColor blackColor];
        markerImage = [UIImage imageNamed:@"Kal.bundle/kal_marker_selected.png"];
    } else if (self.belongsToAdjacentMonth) {
        textColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"Kal.bundle/kal_tile_dim_text_fill.png"]];
        shadowColor = nil;
        markerImage = [UIImage imageNamed:@"Kal.bundle/kal_marker_dim.png"];
    } else {
        textColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"Kal.bundle/kal_tile_text_fill.png"]];
        shadowColor = [UIColor whiteColor];
        markerImage = [UIImage imageNamed:@"Kal.bundle/kal_marker.png"];
    }
    
    if (flags.marked)
        [markerImage drawInRect:CGRectMake(21.f, 5.f, 4.f, 5.f)];
    NSUInteger yearN = [self.date year];
    NSUInteger monthN = [self.date month];
    NSUInteger dayN = [self.date day];
    
    NSString *chineseCalDay = [self caclChineseWithYear:yearN month:monthN day:dayN];
    NSLog(@"chineseCalDay: %@",chineseCalDay);
    
    
    
    
    NSString *dayText = [NSString stringWithFormat:@"%lu", (unsigned long)dayN];
    NSLog(@"dayText: %@",dayText);
    const char *day = [dayText cStringUsingEncoding:NSUTF8StringEncoding];
    CGSize textSize = [dayText sizeWithFont:font];
    CGFloat textX, textY;
    textX = roundf(0.5f * (kTileSize.width - textSize.width));
    textY = 6.f + roundf(0.5f * (kTileSize.height - textSize.height));
    if (shadowColor) {
        [shadowColor setFill];
        CGContextShowTextAtPoint(ctx, textX, textY, day, dayN >= 10 ? 2 : 1);
        textY += 1.f;
    }
    [textColor setFill];
    CGContextShowTextAtPoint(ctx, textX, textY, day, dayN >= 10 ? 2 : 1);
    
    if (self.highlighted) {
        [[UIColor colorWithWhite:0.25f alpha:0.3f] setFill];
        CGContextFillRect(ctx, CGRectMake(0.f, 0.f, kTileSize.width, kTileSize.height));
    }
}

- (NSString *)caclChineseWithYear:(NSUInteger)_year month:(NSUInteger)_month day:(NSUInteger)_day{
    SolarDate solarDate=SolarDate(_year, _month, _day);
    ChineseDate chineseDate;
    //从公历对象转为农历对象
    solarDate.ToChineseDate(chineseDate);
    //获取当年的公历年份对应的农历的年代和甲子年份
    std::pair<int, int> p=ChineseCalendarDB::GetEraAndYearOfLunar(_year);
    
    //设置当前时间的农历日期字段
    NSString *era=[NSString stringWithFormat:@"%d", p.first];
    NSString *year=[NSString stringWithFormat:@"%d", chineseDate.GetYear()];
    NSString *jiazi=[NSString stringWithFormat:@"%d", p.second];
    NSString * animal=[IDJCalendarUtil animalWithJiazi:[jiazi intValue]];
    //获取当前年份的闰月的月份，没有闰月返回0
    NSString *month=[IDJCalendarUtil monthFromCppToMineWithYear:chineseDate.GetYear() month:chineseDate.GetMonth()];
    NSString *day=[NSString stringWithFormat:@"%d", chineseDate.GetDay()];
    
    NSArray *chineseYears=[NSArray arrayWithObjects:@"甲子", @"乙丑", @"丙寅",	@"丁卯",	@"戊辰",	@"己巳",	@"庚午",	@"辛未",	@"壬申",	@"癸酉",
                       @"甲戌",	@"乙亥",	@"丙子",	@"丁丑", @"戊寅",	@"己卯",	@"庚辰",	@"辛己",	@"壬午",	@"癸未",
                       @"甲申",	@"乙酉",	@"丙戌",	@"丁亥",	@"戊子",	@"己丑",	@"庚寅",	@"辛卯",	@"壬辰",	@"癸巳",
                       @"甲午",	@"乙未",	@"丙申",	@"丁酉",	@"戊戌",	@"己亥",	@"庚子",	@"辛丑",	@"壬寅",	@"癸丑",
                       @"甲辰",	@"乙巳",	@"丙午",	@"丁未",	@"戊申",	@"己酉",	@"庚戌",	@"辛亥",	@"壬子",	@"癸丑",
                       @"甲寅",	@"乙卯",	@"丙辰",	@"丁巳",	@"戊午",	@"己未",	@"庚申",	@"辛酉",	@"壬戌",	@"癸亥", nil];
    NSArray *chineseMonths=[NSArray arrayWithObjects:@"正月", @"二月", @"三月", @"四月", @"五月", @"六月", @"七月", @"八月",
                        @"九月", @"十月", @"冬月", @"腊月", nil];
    NSArray * chineseDays=[NSArray arrayWithObjects:@"初一", @"初二", @"初三", @"初四", @"初五", @"初六", @"初七", @"初八", @"初九", @"初十",
                      @"十一", @"十二", @"十三", @"十四", @"十五", @"十六", @"十七", @"十八", @"十九", @"二十",
                      @"廿一", @"廿二", @"廿三", @"廿四", @"廿五", @"廿六", @"廿七", @"廿八", @"廿九", @"三十",  nil];
    
    return day;
    
}

- (void)resetState
{
    // realign to the grid
    CGRect frame = self.frame;
    frame.origin = origin;
    frame.size = kTileSize;
    self.frame = frame;
    
    [date release];
    date = nil;
    flags.type = KalTileTypeRegular;
    flags.highlighted = NO;
    flags.selected = NO;
    flags.marked = NO;
}

- (void)setDate:(KalDate *)aDate
{
    if (date == aDate)
        return;
    
    [date release];
    date = [aDate retain];
    
    [self setNeedsDisplay];
}

- (BOOL)isSelected { return flags.selected; }

- (void)setSelected:(BOOL)selected
{
    if (flags.selected == selected)
        return;
    
    // workaround since I cannot draw outside of the frame in drawRect:
    if (![self isToday]) {
        CGRect rect = self.frame;
        if (selected) {
            rect.origin.x--;
            rect.size.width++;
            rect.size.height++;
        } else {
            rect.origin.x++;
            rect.size.width--;
            rect.size.height--;
        }
        self.frame = rect;
    }
    
    flags.selected = selected;
    [self setNeedsDisplay];
}

- (BOOL)isHighlighted { return flags.highlighted; }

- (void)setHighlighted:(BOOL)highlighted
{
    if (flags.highlighted == highlighted)
        return;
    
    flags.highlighted = highlighted;
    [self setNeedsDisplay];
}

- (BOOL)isMarked { return flags.marked; }

- (void)setMarked:(BOOL)marked
{
    if (flags.marked == marked)
        return;
    
    flags.marked = marked;
    [self setNeedsDisplay];
}

- (KalTileType)type { return flags.type; }

- (void)setType:(KalTileType)tileType
{
    if (flags.type == tileType)
        return;
    
    // workaround since I cannot draw outside of the frame in drawRect:
    CGRect rect = self.frame;
    if (tileType == KalTileTypeToday) {
        rect.origin.x--;
        rect.size.width++;
        rect.size.height++;
    } else if (flags.type == KalTileTypeToday) {
        rect.origin.x++;
        rect.size.width--;
        rect.size.height--;
    }
    self.frame = rect;
    
    flags.type = tileType;
    [self setNeedsDisplay];
}

- (BOOL)isToday { return flags.type == KalTileTypeToday; }

- (BOOL)belongsToAdjacentMonth { return flags.type == KalTileTypeAdjacent; }

- (void)dealloc
{
    [date release];
    [super dealloc];
}

@end
