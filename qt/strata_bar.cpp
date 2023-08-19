// Header
#include "strata_bar.h"

// QT Libraries
#include <QLayout>
#include <QBrush>

// C++ Standard Libraries
#include <iostream>

StrataBar::StrataBar(QWidget* parent, int minimum_, int maximum_) : QWidget(parent)
{
    // Make the widget expanding
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);

    // Add this into the parent's layout
    QLayout* parentLayout = parent->layout();
    parentLayout->addWidget(this);

    strataCount = 0;
    strataPolygons = nullptr;

    this->maximum = maximum_;
    this->minimum = minimum_;
    this->value = 0;
}

StrataBar::~StrataBar()
{
    if(strataPolygons != nullptr) delete [] strataPolygons;
}

void StrataBar::setValue(int value)
{
    // Ignore if no change
    if(this->value == value) return;

    // Set value
    this->value = value;

    // Clamp value
    if(this->value > maximum) this->value = maximum;
    if(this->value < minimum) this->value = minimum;

    // Update appearance
    repaint();
}

void StrataBar::resizeEvent(QResizeEvent* event)
{
    int width = event->size().width();
    int height = event->size().height();
    
    // TODO: Shear
    // int strataShear = height * STRATA_SLOPE;
    // strataCount = static_cast<size_t>((static_cast<double>(width) - strataShear) / (STRATA_SIZE + STRATA_GAP));
    // int remainder = width - strataShear - strataCount * (STRATA_SIZE + STRATA_GAP);
    
    size_t newStrataCount = static_cast<size_t>(static_cast<double>(width) / (STRATA_SIZE + STRATA_GAP));
    
    // Reallocate polygons if necessary
    if(newStrataCount != strataCount)
    {
        strataCount = newStrataCount;

        if(strataPolygons != nullptr) delete [] strataPolygons;
        strataPolygons = new QPolygon[strataCount];
    }
    
    int remainder = width - strataCount * (STRATA_SIZE + STRATA_GAP);

    for(size_t index = 0; index < strataCount; ++index)
    {
        int xPosition = index * (STRATA_SIZE + STRATA_GAP) + remainder / 2;
        
        int x0 = xPosition;
        int x1 = xPosition + STRATA_SIZE;
        int y0 = height - static_cast<double>(height * index) / strataCount;
        int y1 = height - 1;

        // Delete existing polygon
        strataPolygons[index].clear();

        strataPolygons[index] << QPoint(x0, y0);
        strataPolygons[index] << QPoint(x1, y0);
        strataPolygons[index] << QPoint(x1, y1);
        strataPolygons[index] << QPoint(x0, y1);
    }
}

void StrataBar::paintEvent(QPaintEvent*)
{
    // Create painter and set color
    QBrush brush(Qt::green, Qt::SolidPattern);
    QPainter painter(this);
    painter.setPen(Qt::green);
    painter.setBrush(brush);
    
    bool painterIsHighlight = true; // Indicates color of the painter, true => highlight, false => lowlight

    size_t strataValue = static_cast<size_t>((static_cast<double>(value - minimum) / (maximum - minimum)) * strataCount);

    for(size_t index = 0; index < strataCount; ++index)
    {
        // Only one painter may exist per paint device (this), meaning the pen and brush of the paint device must be updated
        // each time the color changes. Kind of annoying, but this is the best solution I could find.
        if(index < strataValue)
        {
            // Paint highlight
            if(!painterIsHighlight)
            {
                brush.setColor(Qt::green);
                painter.setPen(Qt::green);
                painter.setBrush(brush);
                painterIsHighlight = true;
            }
        }
        else
        {
            // Paint lowlight
            if(painterIsHighlight)
            {
                brush.setColor(Qt::red);
                painter.setPen(Qt::red);
                painter.setBrush(brush);
                painterIsHighlight = false;
            }
        }
        
        // Draw strata
        painter.drawConvexPolygon(strataPolygons[index]);
    }
}