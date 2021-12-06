/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "tetrixboard.h"
#include "tetrixwindow.h" //board!!

#include <QCoreApplication>
#include <QGridLayout>

// ---------------------------
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QString>
//! [0]
//las siguientes dos lineas son para inicializar, el TetrixBoard para el "playing area"
//tambien el widget parent para inicializarlo
TetrixWindow::TetrixWindow(QWidget *parent)
    : QWidget(parent), board(new TetrixBoard)
{
    setStyleSheet("background-color:black;");
//! [0]
//  NEXT PIECE label
    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    board->setNextPieceLabel(nextPieceLabel);
//! [1]
//  SCORE en 5 digits initialization
    scoreLcd = new QLCDNumber(5);//digits number
    scoreLcd->setSegmentStyle(QLCDNumber::Filled);
    scoreLcd->setStyleSheet(
                "background-color:#064635;"
                "color: #FFEBCC;"
                "border-color: #5584AC;"
                "border-style: dashed;"
                " border-width: 5px;"
                );
//! [1]
//  LEVEL of the game
    levelLcd = new QLCDNumber(2);//digits number
    levelLcd->setSegmentStyle(QLCDNumber::Flat);//style of numbers
    levelLcd->setStyleSheet(
                "background-color:#064635;"
                "color: #FFEBCC;"
                "border-color: #5584AC;"
                "border-style: dashed;"
                " border-width: 5px;"
                );
//  LINEAS borradas
    linesLcd = new QLCDNumber(5);//digits number
    linesLcd->setStyleSheet(
                "background-color:#064635;"
                "color: #FFEBCC;"
                "border-color: #5584AC;"
                "border-style: dashed;"
                " border-width: 5px;"
                );
    linesLcd->setSegmentStyle(QLCDNumber::Outline);

//! [2]
    startButton = new QPushButton(tr("&Start"));//tr -> means "tranlated"
    startButton->setStyleSheet("color: white;"
                               "background-color: #22577E;"
                                "border-color: #F6F2D4;"
                               "border-radius: 12px;"
    );

    //by default no permit focus on pushButton element
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setStyleSheet("color: white;"
                              "background-color: #22577E;"
                               "border-color: #F6F2D4;"
                              "border-radius: 12px;"
    );
    quitButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setStyleSheet("color: white;"
                               "background-color: #22577E;"
                                "border-color: #F6F2D4;"
                               "border-radius: 12px;"
    );
//! [2] //! [3]
    pauseButton->setFocusPolicy(Qt::NoFocus);
//! [3] //! [4]

    //START button
    connect(startButton, //my sender
            &QPushButton::clicked,//my SIGNAL
            board,//my receiver
            &TetrixBoard::start); //my SLOT
//! [4] //! [5]
//! QUIT BUTTON
    connect(quitButton ,
            &QPushButton::clicked,
            qApp,
            &QCoreApplication::quit);
//  PAUSE button --> Tetrix button pause
    connect(pauseButton, //push button
            &QPushButton::clicked, //SIGNAL
            board, // tetrixWindow -> board,  Receiver
            &TetrixBoard::pause); //SLOT TetrixBoard -> pause

    connect(this,
            &TetrixWindow::selectRadioButtonChanged,
            board,
            &TetrixBoard::updateMethod
    );
/*
 * connect(radio3,
            &QRadioButton::clicked,
            this,
            &TetrixWindow::updateMethodBoard
    );
    connect(dificultBox,
            &QGroupBox::clicked(false),
            board,
            setDificultLevel()
            );
*/


#if __cplusplus >= 201402L
    connect(board, //sender
            &TetrixBoard::scoreChanged, //tetrix board
            scoreLcd, //sender
            //overload function display()[int,double,string]
            //for use specific function, you could use "QOverloadObject"
            qOverload<int>(&QLCDNumber::display)
            );
    connect(board, &TetrixBoard::levelChanged,
            levelLcd, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, qOverload<int>(&QLCDNumber::display));


    //emit selectRadioButtonChanged("bastard");

#else
    connect(board, &TetrixBoard::scoreChanged,
            scoreLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::levelChanged,
            levelLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, QOverload<int>::of(&QLCDNumber::display));
#endif
//! [5]

//! [6]

    QGridLayout *layout = new QGridLayout;
    //layout->addWidget(createLabel(tr("NEXT")), 0, 0);
    layout->addWidget(createBox(),1, 0);
    //layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(createLabel(tr("Nivel")), 2, 0);
    layout->addWidget(levelLcd, 3, 0);
    layout->addWidget(startButton, 4, 0);
    //board->setFocus();
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(createLabel(tr("SCORE")), 0, 2);
    layout->addWidget(scoreLcd, 1, 2);
    layout->addWidget(createLabel(tr("LINEAS REMOVIDAS")), 2, 2);
    layout->addWidget(linesLcd, 3, 2);
    layout->addWidget(quitButton, 4, 2);
    layout->addWidget(pauseButton, 5, 2);

    setLayout(layout);

    setWindowTitle(tr("Tetrix"));
    resize(900, 600);
}
//! [6]

//! [7]
QLabel* TetrixWindow::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label->setStyleSheet(
                "color: #FFE6BC;"
                "font-size: 34px;"
                "font-family: 'Courier New';"
                );
    return label;
}

//! [7]
QGroupBox* TetrixWindow::createBox(){
    if(dificultBox != nullptr){
        delete dificultBox;
    }
    dificultBox = new QGroupBox(tr("Nivel de dificultad"));
    QVBoxLayout *vbox = new QVBoxLayout;
    radio1 = new QRadioButton(tr("Bastard"));
    radio2 = new QRadioButton(tr("Normal"));
    radio3 = new QRadioButton(tr("Nice"));
//    dificultBox->children();

    connect(radio1,
            &QRadioButton::clicked,
            this,
            &TetrixWindow::updateMethodBoard
    );

    connect(radio2,
            &QRadioButton::clicked,
            this,
            &TetrixWindow::updateMethodBoard
    );
    connect(radio3,
            &QRadioButton::clicked,
            this,
            &TetrixWindow::updateMethodBoard
    );

    radio1->setChecked(true);

    radio1->setStyleSheet(
                "color: #DD4A48;"
                "font-family: 'Courier New';"
                "font-size: 24px;"
    );
    radio2->setStyleSheet(
                //DD4A48
                            "color: #FFCA03;"
                            "font-family: 'Courier New';"
                            "font-size: 24px;"
           );
    radio3->setStyleSheet(
                //DD4A48
                            "color: #71DFE7;"
                            "font-family: 'Courier New';"
                            "font-size: 24px;"
           );
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    //vbox->addStretch(1);
    dificultBox->setStyleSheet(
                "color: #396EB0;"
                "font-family: 'Courier New';"
                "font-size: 24px;"
                );
    dificultBox->setLayout(vbox);
    dificultBox->setFocusPolicy(Qt::ClickFocus);

    return dificultBox;
}
void TetrixWindow::updateMethodBoard(){
    if(radio1->isChecked()){
        emit selectRadioButtonChanged("1");
    }else
    if(radio2->isChecked()){
        emit selectRadioButtonChanged("2");
    }else
    if(radio3->isChecked()){
        emit selectRadioButtonChanged("3");
    }else{
        emit selectRadioButtonChanged("1");
    }
}


