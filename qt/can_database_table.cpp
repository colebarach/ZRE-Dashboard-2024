// Header
#include "can_database_table.h"

// QT Libraries
#include <QString>
#include <QVBoxLayout>

// C++ Standard Libraries
#include <exception>
#include <stdexcept>

CanDatabaseTable::CanDatabaseTable(QWidget* parent, Network::CanDatabase* database_) : QWidget(parent)
{
    database = database_;

    // Make the widget expanding
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add this into the parent's layout
    QLayout* parentLayout = parent->layout();
    if(parentLayout == nullptr) throw std::runtime_error("Failed to create CanDatabaseTable: The parent widget has no layout.");
    parentLayout->addWidget(this);

    // canMessages  = database->getMessages();
    // canSignals   = database->getSignals();
    // messageCount = database->getMessageCount();
    // signalCount  = database->getSignalCount();

    // messageLabels = new QLabel*[messageCount];
    // signalLabels  = new QLabel*[signalCount];

    QVBoxLayout* layout_ = new QVBoxLayout(this);

    // // Populate widget
    // for(size_t messageIndex = 0; messageIndex < messageCount; ++messageIndex)
    // {
    //     const Network::CanMessage& message = canMessages[messageIndex];
        
    //     messageLabels[messageIndex] = new QLabel(message.name, this);
    //     layout_->addWidget(messageLabels[messageIndex]);

    //     for(size_t signalIndex = message.signalIndex; signalIndex < message.signalCount + message.signalIndex; ++signalIndex)
    //     {
    //         const Network::CanSignal& signal = canSignals[signalIndex];

    //         signalLabels[signalIndex] = new QLabel(signal.name, this);
    //         layout_->addWidget(signalLabels[signalIndex]);
    //     }
    // }


    // ----
    testLabel = new QLabel("*Insert messages here*", this);
    layout_->addWidget(testLabel);
    // ----


    // Apply the layout
    // - The dynamic object does not need deleted, as the QWidget takes ownership of it
    setLayout(layout_);
}

CanDatabaseTable::~CanDatabaseTable()
{
    // // Delete allocated widgets
    // for(size_t index = 0; index < messageCount; ++index)
    // {
    //     delete messageLabels[index];
    // }
    // for(size_t index = 0; index < signalCount; ++index)
    // {
    //     delete signalLabels[index];
    // }

    // delete [] messageLabels;
    // delete [] signalLabels;
}

void CanDatabaseTable::update()
{
    std::string text;
    database->print(&text);
    testLabel->setText(QString::fromStdString(text));
}