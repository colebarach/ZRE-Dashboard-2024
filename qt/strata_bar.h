#ifndef STRATA_BAR_H
#define STRATA_BAR_H

// Strata Bar -----------------------------------------------------------------------------------------------------------------
//
// Description: A stylized progress bar. This class is designed to mock the QProgressBar class, but is is not polymorphic with
//   it. Full polymorphism would be nice, but requires the creation of the entire widget, which isn't desired.
//
// Created: 23.08.12
// Updated: 23.08.17

// Libraries ------------------------------------------------------------------------------------------------------------------

// QT Libraries
#include <QWidget>
#include <QFrame>
#include <QResizeEvent>
#include <QPolygon>
#include <QPainter>

class StrataBar : QWidget
{
    Q_OBJECT // QT object boilerplate

    // Constants --------------------------------------------------------------------------------------------------------------

    #define STRATA_SIZE  8
    #define STRATA_GAP   4
    #define STRATA_FILL_MULTIPLIER 0.5
    // #define STRATA_SLOPE 0.5

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    StrataBar(QWidget* parent, int minimum_ = 0, int maximum_ = 100);

    ~StrataBar();

    // Accessors --------------------------------------------------------------------------------------------------------------

    int getValue() const { return this->value; }

    int getMaximum() const { return this->maximum; }

    int getMinimum() const { return this->minimum; }

    void setValue(int value);

    void setMaximum(int maximum) { this->maximum = maximum; }

    void setMinimum(int minimum) { this->minimum = minimum; }

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    QPolygon* strataPolygons;
    size_t strataCount;

    int value;
    int maximum;
    int minimum;

    // QT Events --------------------------------------------------------------------------------------------------------------

    protected:

    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent* paintEvent) override;
};

#endif // STRATA_BAR_H