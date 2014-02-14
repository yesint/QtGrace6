#ifndef SKFSTYLE_H
#define  SKFSTYLE_H

#if QT_VERSION < 0x050000

#include <QPlastiqueStyle>
#include <QCleanlooksStyle>
#include <QWindowsStyle>

#if QT_VERSION < 0x040500
#define MYSTYLE QWindowsStyle
#else
#define MYSTYLE QPlastiqueStyle
#endif

#else
#include <QCommonStyle>
#include <QStyle>
#define MYSTYLE QCommonStyle

#endif


class BeautyStyle:public MYSTYLE
{
public:
    BeautyStyle(int marginIn,int spacingIn):MYSTYLE(),margin(marginIn),spacing(spacingIn) {};
    virtual ~BeautyStyle(){};
    int margin;
    int spacing;
    virtual int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const {
 

        // BTRACEF(Int2Str(QStyle::PM_SmallIconSize)); // STRANGE IN 67.73

        int retval=MYSTYLE::pixelMetric(metric,option,widget);

        if (retval!=-1){
            
        

            //   if ((margin<0)||(spacing<0)) return MYSTYLE::pixelMetric(metric,option,widget);

            if (metric==QStyle::PM_LayoutTopMargin) retval=margin;
            if (metric==QStyle::PM_LayoutRightMargin) retval=margin;
            if (metric==QStyle::PM_LayoutBottomMargin) retval= margin;
            if (metric==QStyle::PM_LayoutLeftMargin) retval= margin;
            if (metric==QStyle::PM_LayoutHorizontalSpacing) retval= spacing;
            if (metric==QStyle::PM_LayoutVerticalSpacing) retval= spacing;
            if (metric==QStyle::PM_ButtonMargin) retval= spacing;
            if (metric==QStyle::PM_ToolBarIconSize) retval=32;
            if (metric==QStyle::PM_MenuBarItemSpacing) retval=20;
            // if (metric==77) retval= spacing;
        }
 
        return retval;
    };
};
#endif
