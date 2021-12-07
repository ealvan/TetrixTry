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

#ifndef TETRIXBOARD_H
#define TETRIXBOARD_H

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
//cuadraditos, como una matriz
#include "tetrixpiece.h"

QT_BEGIN_NAMESPACE
class QLabel;//forward declarations,
QT_END_NAMESPACE

//! [0]
class TetrixBoard : public QFrame
{
    Q_OBJECT

private:
    // GAME LOGIC of TETRIX

    enum { BoardWidth = 10, BoardHeight = 22 };
    QBasicTimer timer;//para controlar la velocidad con la que cae la pieza
    QPointer<QLabel> nextPieceLabel;//DELnextpiece label
    bool comienzo;
    bool isPaused;
    bool waitSgteLine;
    TetrixPiece curPiece;//current Piece
    TetrixPiece nextPiece;//next Piece del enum
    int curX;
    int curY;
    int numLinesRemoved;
    int numPiecesDropped;
    int score;
    int level;
    int coorde[20]={0};
    void getCoordinate(int top_level );
    TetrixShape board[BoardWidth * BoardHeight];
    TetrixShape& shapeAt(int x, int y) {
        //te pone las coordenadas de board
        //en el tablero, la esquina superior izquierda
        return board[ x +(y * BoardWidth)];
        //X: celda
        //Y:alrtura donde esta esa celda
    }
    int method=1;
    TetrixShape bastard();
    void getOfficeHeight();
    TetrixShape getRamdomHardPiece(std::map<TetrixShape,int>& mapaHard);
    void getMaxOnMap(std::map<TetrixShape,int>& mapaHard,int&max);
    void deleteShapesOnMap(std::map<TetrixShape,int>& mapaHard,int&max);
    bool isbastard = false;

    //es usado para el start(), para reanudar cuando esta pausado
    int timeoutTime() {
        if(method == 1 && isbastard){
            return 300 / (1 + level);//masnivel, mas rapido
        }else if(method==2 && !isbastard){
            return 800 / (1 + level);
        }else{
            return 1200 / (1 + level);
        }
    }
    int squareWidth() {
        return contentsRect().width() / BoardWidth;
    }
    int squareHeight() {
        return contentsRect().height() / BoardHeight;
    }
    void clearBoard();//clear board
    void dropDown();
    void oneLineDown();
    void pieceDropped(int dropHeight);
    void removeFullLines();
    void newPiece();
    void showNextPiece();
    bool tryMove(const TetrixPiece &newPiece, int newX, int newY);
    void drawSquare(QPainter &painter, int x, int y, TetrixShape shape);
    void printState();
public:
    TetrixBoard(QWidget *parent = nullptr);
    int getTrueHeight(std::vector<int>& rows);//MIO
    bool rowIsNoShape(int row);
    void setNextPieceLabel(QLabel *label);
    QSize sizeHint() const override;//tamaño del widtget
    QSize minimumSizeHint() const override;//tamaño minimo recomendado,
    //si su size esta mal, enviara un valor invalido

public slots:
    void start();//son los que se ejcutaran dado una SIGNAL
    void pause();
    void updateMethod(QString methodS);
signals://son las señales que puede enviar esta clase
    void scoreChanged(int score);//si cambia el score
    void levelChanged(int level);//si sube de nivel
    void linesRemovedChanged(int numLines);//si elimino mas lineas

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    //esta reimplementacion nos ayuda a que pueda el objeto recibir evento de timer
    //es decir "TimerEvent"
    void timerEvent(QTimerEvent *event) override;//el QBasicTime va de la mano con este
//! [0]

//! [1]

};
//! [1]

#endif
