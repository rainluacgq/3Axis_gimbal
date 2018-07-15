#ifndef TIMESCALEDRAW_H
#define TIMESCALEDRAW_H
#include <QMainWindow>
#include <math.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qdatetime.h>
#include <QDateTime>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <QPoint>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
class TimeScaleDraw : public QwtScaleDraw
{
public:
    TimeScaleDraw(const QDateTime &base):baseTime(base){
    }

    virtual QwtText label(double v) const
    {
        QDateTime upTime = baseTime.addSecs((int )v);
    //    return upTime.toString("yyyy-MM-dd\nhh:mm:ss");
        return upTime.toString("hh:mm:ss");
    }


private:
    QDateTime baseTime;
};
#endif // TIMESCALEDRAW_H
