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

#ifndef TETRIXWINDOW_H
#define TETRIXWINDOW_H

#include <QWidget>
QT_BEGIN_NAMESPACE
class QRadioButton;
class QVBoxLayout;
class QLCDNumber;
class QLabel;
class QPushButton;
class QGroupBox;
QT_END_NAMESPACE
class TetrixBoard;

//! [0]
class TetrixWindow : public QWidget
{
    Q_OBJECT

public:
    //constructor, no provee QWidget para ser especificado,
    //porque TetrsiWindow, sera el top level window!
    TetrixWindow(QWidget *parent = nullptr);
signals:
    //1,2,3
    void selectRadioButtonChanged(QString);
public slots:
    void updateMethodBoard();

private:
    QLabel *createLabel(const QString &text);
    QGroupBox* createBox();
    TetrixBoard *board;//contiene la logica del juego, hereda de QFrame
    QLabel *nextPieceLabel; //siguiente pieza para el display
    QLCDNumber *scoreLcd; //es la puntucion o score para incializar
    QLCDNumber *levelLcd; //es el nivel en el que esta el usuario
    QLCDNumber *linesLcd; //lineas removidas por el usuario
    QPushButton *startButton;//boton de inicio
    QPushButton *quitButton;//boton de salir del juego
    QPushButton *pauseButton; // boton de pausar
//    QVBoxLayout *dificulBox;//contiene radio buttons
    QGroupBox *dificultBox = nullptr;
//    Material.theme: Material.Dark
    QRadioButton *radioBastard;//radioButton que elige el metodo Bastard
    QRadioButton *radioNormal;//radioButton que elige el metodo Normal
    QRadioButton *radioEasy;//radioButton que elige el metodo Easy o Nice

};
//! [0]

#endif
