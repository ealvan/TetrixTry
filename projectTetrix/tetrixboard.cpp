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
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QBrush>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <QRandomGenerator>

//! [0]
// El tetrixBoard es una subclase de QFrame,
// y hay un QRect que cubre toda la zona,
// antes de pintar esa zona
TetrixBoard::TetrixBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    //cambia el style del playing area
    setFrameStyle(QFrame::Box | QFrame::Sunken);
    //set focus policy, nos permite que recnozca el tapping
    //o el clicking pero sino tienes el StringFocus o el TabFocus
    //no re permite usar la ficha
    //setFocusPolicy(Qt::TabFocus);
    setFocusPolicy(Qt::TabFocus);
    //es un metodo que limpia el board
    clearBoard();//inicializa a NoShape

    nextPiece.setRandomShape();//el enxt Piece es random
}
//! [0]

//! [1]
void TetrixBoard::setNextPieceLabel(QLabel *label)
{
    //es el setNextPieceLabel
    nextPieceLabel = label;
}
//! [1]

//! [2]
QSize TetrixBoard::sizeHint() const
{

    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);

}

QSize TetrixBoard::minimumSizeHint() const
//! [2] //! [3]
{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

void TetrixBoard::updateMethod(const QString& method){


}



//! [3]
void TetrixBoard::newPiece(){

    //qDebug() << "AAAAAAAAA"<<"  \n";
    //TetrixPiece test;
    //test.setShape(LineShape);
    //antes de esto, en el constructor se inicializa nextPiece, con setRandomPiece()
    //printState();
    int method=1;
    TetrixPiece dump;
    TetrixShape hard;
    TetrixPiece pieza;
    switch (method) {
    case 1:{
        timer.stop();
        isbastard = true;//nivel dificil
        hard = bastard();
        std::cout << "hard: "<<hard <<"\n";
        timer.start(timeoutTime(), this);
    }
    break;
    case 2:{
        dump.setRandomShape();
        hard = dump.shape();
    }
    break;
      case 3:{
        timer.stop();
        isbastard = false;
        hard = bastard();
        timer.start(timeoutTime(), this);
    }
    break;
    }
    pieza.setShape(hard);

    //curPiece = nextPiece;//copia los valores del nextPiece al curPiece
    curPiece = pieza;

    //nextPiece.setRandomShape();//ademas el nextPiece rellena su atributo coords[4][2]
    //showNextPiece();
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.minY();

    if (!tryMove(curPiece, curX, curY)) {
        curPiece.setShape(NoShape);
        timer.stop();
        isStarted = false;
    }
//! [30] //! [31]
}
void TetrixBoard::showNextPiece()
{
    //if(!nextPieceLabel) return;
    if (nextPieceLabel == nullptr)
        return;

    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;
    //lo que estamos pintando en realidad es la pieza de tetris
    //no el mapa
    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
    //QPaintDevice <- QPixmap
    QPainter painter(&pixmap);//capta un QPaintDevice
    QBrush mybrush;
    mybrush.setStyle(Qt::SolidPattern);
    //painter.fillRect(pixmap.rect(), nextPieceLabel->palette().window());
    painter.fillRect(pixmap.rect(), mybrush);

    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();

        drawSquare(painter,
                   x * squareWidth(),
                   y * squareHeight(),
                   nextPiece.shape()
        );
    }
    nextPieceLabel->setPixmap(pixmap);
//! [32] //! [33]
}

//! [36]
void TetrixBoard::drawSquare(QPainter &painter, int x, int y, TetrixShape shape){
    static constexpr QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };
    int random = QRandomGenerator::global()->bounded(7) + 1;

    QColor color = colorTable[random];
    painter.fillRect(x + 1,
                     y + 1,
                     squareWidth() -2,
                     squareHeight() -2,
                     color);

    painter.setPen(color.black());
    /**/
    painter.drawLine(x, y + squareHeight() - 1, x, y);

    //painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.darker());

    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);

    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

//ESTA FABRICA TODAS LAS COORDENADAS
void TetrixBoard::getOfficeHeight(){
    //timer.stop();
    std::vector<int> cuadrados(10,int{});
    cuadrados.reserve(10);
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetrixShape shape = shapeAt(j, BoardHeight - i - 1);
            if(shape == NoShape){
               // std::cout << int(shape);
                cuadrados[j] += 0;
            }else{
              // std::cout << int(shape);
                cuadrados[j] += 1;
            }
        }
       //std::cout <<"\n";
    }
    //std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    int trueH = getTrueHeight(cuadrados);
    getCoordinate(trueH);
    //timer.stop();
    //timer.start(timeoutTime(), this);
}

//! [36]
void TetrixBoard::printState(){
    ////timer.stop(); timer.start(timeoutTime(), this);
    timer.stop();
    std::vector<int> cuadrados(10,int{});
    cuadrados.reserve(10);


    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetrixShape shape = shapeAt(j, BoardHeight - i - 1);
            if(shape == NoShape){
                std::cout << int(shape);
                cuadrados[j] += 0;
            }else{
               std::cout << int(shape);
                cuadrados[j] += 1;
            }
        }
       std::cout <<"\n";
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    int trueH = getTrueHeight(cuadrados);

    std::cout <<"Altura Verdadera: "<<trueH<< "\n";
    //aqui no tenemos nada
    getCoordinate(trueH);

    int i=0;
    for(int& item: coorde){
        std::cout <<i<< ": "<<item<<"\n";
        ++i;
    }




    timer.start(timeoutTime(), this);
    //pause();
}

void TetrixBoard::getCoordinate(int top_level){
    if(top_level == 1){
        //init a cero
        return;
    }

    int cor = 0;

    for(int i = 0 ; i < BoardWidth; ++i){
        for(int j=top_level; j >= 0 ; --j){
            TetrixShape shape = shapeAt(i,j-1);
            if(shape != NoShape || j == 0){
                coorde[cor] = i;
                cor = cor +1;
                coorde[cor] = j;
                cor = cor +1;
                break;
            }
        }
    }
}
int TetrixBoard::getTrueHeight(std::vector<int>& rows){
    auto max = std::max_element(rows.begin(),rows.end());
    int alturaMax = *max;
    //std::cout << "Max: 0"<<alturaMax<<"\n";
    bool isTrueH = rowIsNoShape(alturaMax+1);//la sigte fila de la altura maxima debe estar llena de ceros

    if(isTrueH){
        return alturaMax+1;
    }else{
        int upper = 1;
        while(!rowIsNoShape(alturaMax+upper)){
            upper++;
        }
        return alturaMax+upper;
    }
}
bool TetrixBoard::rowIsNoShape(int row){
    bool isNoShape = true;
    for(int j=0; j < BoardWidth; ++j){
        TetrixShape shape = shapeAt(j,row-1);
        if(shape != NoShape){
           isNoShape = false;
           break;
        }
//        std::cout << int(shape);
    }
    return isNoShape;
}
//! [4]
void TetrixBoard::start() {

    if (isPaused)
        return;

    isStarted = true;
    isWaitingAfterLine = false;
    numLinesRemoved = 0;
    numPiecesDropped = 0;
    score = 0;
    level = 1;
    clearBoard();

    emit linesRemovedChanged(numLinesRemoved);
    emit scoreChanged(score);
    emit levelChanged(level);

    newPiece();
    timer.start(timeoutTime(), this);
}
//! [4]

//! [5]
void TetrixBoard::pause()
{
    //printState();
    if (!isStarted)
        return;
    isPaused = !isPaused;
    if (isPaused) {
        timer.stop();
    } else {
        timer.start(timeoutTime(), this);
    }
    //update();//cuando no lo pongo, se pausa tood, pero no muestra el mensaje Paused
    //update();
//! [5] //! [6]
}
//! [6]

//! [7]
//!
void TetrixBoard::paintEvent(QPaintEvent *event) {
    //printState();// mala idea
    QFrame::paintEvent(event);//no entiendo

    QPainter painter(this);//
    QRect rect = contentsRect();
//! [7]

    if (isPaused) {
/*
drawText(const QRect &rectangle, int flags, const QString &text, QRect *boundingRect = nullptr)
*/
        painter.drawText(rect, Qt::AlignCenter, tr("Pause"));
        return;
    }

//! [8]
    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetrixShape shape = shapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }
//! [8] //! [9]
    }
//! [9]

//! [10]
    if (curPiece.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            drawSquare(painter,
                       rect.left() + x * squareWidth(),
                       boardTop + (BoardHeight - y - 1) * squareHeight(),
                       curPiece.shape()
            );
        }
//! [10] //! [11]
    }
//! [11] //! [12]
}
//! [12]

//! [13]
void TetrixBoard::keyPressEvent(QKeyEvent *event)
{
    if (!isStarted || isPaused || curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }
//! [13]

//! [14]
    switch (event->key()) {
        case Qt::Key_Left:
            tryMove(curPiece, curX - 1, curY);
            break;
        case Qt::Key_Right:
            tryMove(curPiece, curX + 1, curY);
            break;
        case Qt::Key_Down:
            tryMove(curPiece.rotatedRight(), curX, curY);
            break;
        case Qt::Key_Up:
            tryMove(curPiece.rotatedLeft(), curX, curY);
            break;
        case Qt::Key_Space:
            dropDown();
            break;
        case Qt::Key_D:
            oneLineDown();
            break;
        default:
            QFrame::keyPressEvent(event);
    }
//! [14]
}

//! [15]
void TetrixBoard::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        if (isWaitingAfterLine) {
            isWaitingAfterLine = false;

            newPiece();
            timer.start(timeoutTime(), this);
        } else {
            oneLineDown();
            //std::cout << "BBBBBBBBBB"<<std::endl;
        }
    } else {
        QFrame::timerEvent(event);
        //std::cout << "BBBBBBBBBB"<<std::endl;
//! [15] //! [16]
    }
//! [16] //! [17]
}
//! [17]

//! [18]
void TetrixBoard::clearBoard()
{
    //el board se limpia e inicializa con
    //board[i] con NoShape, cada bloque
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        board[i] = NoShape;
}
//! [18]

//! [19]
void TetrixBoard::dropDown() // Space key
{
    int dropHeight = 0;
    int newY = curY;
    while (newY > 0) {
        if (!tryMove(curPiece, curX, newY - 1))
            break;
        --newY;
        ++dropHeight;
    }
    pieceDropped(dropHeight);
//! [19] //! [20]
}
//! [20]

//! [21]
void TetrixBoard::oneLineDown()// key D
{
    if (!tryMove(curPiece, curX, curY - 1))
       pieceDropped(0);
}
//! [21]

//! [22]
void TetrixBoard::pieceDropped(int dropHeight)
{
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        shapeAt(x, y) = curPiece.shape();
    }

    //std::cout << "AHHH\n";
    ++numPiecesDropped;
    if (numPiecesDropped % 25 == 0) {
        ++level;
        timer.start(timeoutTime(), this);
        emit levelChanged(level);
    }

    score += dropHeight + 7;
    emit scoreChanged(score);
    removeFullLines();

    if (!isWaitingAfterLine)
    {

        //printState();
        newPiece();
    }
//! [22] //! [23]
}
//! [23]

//! [24]
void TetrixBoard::removeFullLines()
{
    int numFullLines = 0;

    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {
            if (shapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
//! [24] //! [25]
            ++numFullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j)
                    shapeAt(j, k) = shapeAt(j, k + 1);
            }
//! [25] //! [26]
            for (int j = 0; j < BoardWidth; ++j)
                shapeAt(j, BoardHeight - 1) = NoShape;
        }
//! [26] //! [27]
    }
//! [27]

//! [28]
    if (numFullLines > 0) {
        numLinesRemoved += numFullLines;
        score += 10 * numFullLines;
        emit linesRemovedChanged(numLinesRemoved);
        emit scoreChanged(score);

        timer.start(500, this);
        isWaitingAfterLine = true;
        curPiece.setShape(NoShape);
        update();
    }
//! [28] //! [29]
}
//! [29]

//! [34]
//!
//!
TetrixShape TetrixBoard::bastard(){

    //aqui se usa la array coorde[]

    getOfficeHeight();
    std::map<TetrixShape,int> mapaHard ={
        {ZShape,0},//1
        { SShape,0},//2
        {LineShape,0},//3
        {TShape,0},//4
        {SquareShape,0},//5
        {LShape,0},//6
        {MirroredLShape,0}//7
    };
    //tenemos el array con corrdenadas//si qnecesitamos la altura
    //std::set<TetrixShape> listaHard;

   // listaHard.reserve(7);//7 piezas

    //1...7
    for(int i=0; i < 19;i+=2 ){
        int x = coorde[i];//0,2,4,
        int y = BoardHeight - coorde[i+1];//??????????????????

        TetrixShape forma = TetrixShape::ZShape;

        for(int j = int(forma); j < 8; ++j){//recorremos por shape
            forma = TetrixShape(j);//teneoms todas,L,S
            TetrixPiece ficha;
            ficha.setShape(forma);//normal//orignal

            bool arr[4]={false};
            //bool sePuede = tryMove(ficha,x,y);
            for(int k= 0; k<4; ++k ){
                arr[k] = tryMove(ficha,x,y);
                //----------
                ficha = ficha.rotatedLeft();
            }
            //---ANALISIS
            //si es >= a 3 los falsos
            //lo elegimos
            //sino
            //false, true,true,true
            //
            int count=0;
            for(int m = 0; m < 4; ++m){
                if(arr[m] == (!isbastard)){
                    count+=1;
                }
            }
            if(count >= 4){
                mapaHard[forma]+=1;
                //4 trues ->facil
                //3 trues ->+-
                //2 trues -> intermedio
                //1 trues ->dificil
            }
        }
    }
    //aca..................mapa
    /*
    std::map<TetrixShape,int>::iterator iter;
    int max = 0;
    TetrixShape elegida = TetrixShape::ZShape;
    for (iter = mapaHard.begin(); iter != mapaHard.end(); iter++){
        std::cout <<"LLave: "<<iter->first<<" Vlaue: "<<iter->second<<"\n";
        if(max < iter->second){
            max = iter->second;
            elegida = iter->first;
        }
    }

    */
    /*

    std::map<TetrixShape,int>::iterator iter;
    //mayor dificultad //pero
    for(iter = mapaHard.begin(); iter != mapaHard.end(); iter++){
        if(iter->second < max){
            mapaHard.erase(iter->first);
        }
    }
auto it = m.begin();
std::advance(it, rand() % m.size());
K random_key = it->first;
    */
/*
    srand(time(NULL));
//QRandomGenerator::global()->bounded(mapaHard.size()) + 1)
    std::map<TetrixShape,int>::iterator it = mapaHard.begin();
    std::advance(it,rand()%mapaHard.size());
    TetrixShape ramdon_key = TetrixShape(it->first);
*/
    int max = 0;
    getMaxOnMap(mapaHard,max);
    deleteShapesOnMap(mapaHard,max);//este!!
    TetrixShape ramdon_key = getRamdomHardPiece(mapaHard);
    if(ramdon_key == curPiece.shape()){
        if(mapaHard.size() > 1){
            mapaHard.erase(ramdon_key);
            ramdon_key = getRamdomHardPiece(mapaHard);
        }
    }

    return ramdon_key;
}
void TetrixBoard::getMaxOnMap(std::map<TetrixShape,int>& mapaHard,int&max){
    std::map<TetrixShape,int>::iterator iter;
    TetrixShape elegida = TetrixShape::ZShape;
    for (iter = mapaHard.begin(); iter != mapaHard.end(); iter++){
        std::cout <<"LLave: "<<iter->first<<" Vlaue: "<<iter->second<<"\n";
        if(max < iter->second){
            max = iter->second;
            elegida = iter->first;
        }
    }
//    return elegida;
}
void TetrixBoard::deleteShapesOnMap(std::map<TetrixShape,int>& mapaHard,int&max){
    std::map<TetrixShape,int>::iterator iter;
    //mayor dificultad //pero
    for(iter = mapaHard.begin(); iter != mapaHard.end(); iter++){
        if(iter->second < max){
            mapaHard.erase(iter->first);//1
        }
    }
}
TetrixShape TetrixBoard::getRamdomHardPiece(std::map<TetrixShape,int>& mapaHard){
    srand(time(NULL));
//QRandomGenerator::global()->bounded(mapaHard.size()) + 1)
    std::map<TetrixShape,int>::iterator it = mapaHard.begin();
    std::advance(it,rand()%mapaHard.size());
    TetrixShape ramdon_key = TetrixShape(it->first);
    return ramdon_key;
}
// DIFICIL

//funcion
// FACIL




bool TetrixBoard::tryMove(const TetrixPiece &newPiece, int newX, int newY)
{
    //std::cout << "newX: "<<newX <<"\tnewY: "<<newY<<std::endl;
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (shapeAt(x, y) != NoShape)
            return false;
    }
//! [34]

//! [35]
    curPiece = newPiece;
    curX = newX;
    curY = newY;
    update();
    return true;
}
//! [35]

