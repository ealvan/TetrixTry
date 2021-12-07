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
    : QFrame(parent), comienzo(false), isPaused(false)
{
    //cambia el style del playing area
    setFrameStyle(QFrame::Box | QFrame::Sunken);
    //set focus policy, nos permite que recnozca el tapping
    //ahay politicas de Focus, en la documentacion
    //es necesario usar el TabFocus, porque asi hacemos que
    //se enfoque en este widget con el TAB
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
//Estos metodos son metodos que nos retornan
//el Size del widget, que quieres cuando se muestre toda la aplicacion
QSize TetrixBoard::sizeHint() const
{

    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);
}
//este tambn es un override method
//porque nos da el minimunsize Recomendado
//si no e posible asignar este szeHint,
//el minimun sizeHint hace su trabajo
QSize TetrixBoard::minimumSizeHint() const
//! [2] //! [3]
{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

//este metodo recibe desde el slot de TetrixWindow
//un Qstring que nos da el metodo elegiio el usuario
void TetrixBoard::updateMethod(QString methodS){
    //si el usuario presiona
    /*
    1: Bastard  -> el mas dificil
    2: Normal -> eeste usa una random TetrixPiece
    3: Nice -> este usa el metodo BastardTetrix PERO al contrario
    //es decir recolecta las piezas que mas encajes tengan

    //el nice tetris muchas veces elige
    //los palos o los cuadrados, cuando elige las ZShape o LShape
    //la mayoria de los casos es por que tiene bastantes huecos
    //su implementacion

    //pero es la contrario con Bastard Tetrix, usa la pero pieza
    */
    method = methodS.toInt();
}



//! [3]
void TetrixBoard::newPiece(){

    //este metodo  crea una nueva pieza, y es muy usado
    //Ademas este metodo tambien tiene una parte de codigo
    //para elgegir el metodo o nivel de dificultad
    TetrixPiece dump;
    //es la forma de la pieza que
    //botara nuestro algoritmo
    TetrixShape hard;
    TetrixPiece pieza;//es la pieza qe se pondra
    switch (method) {//method es un miebro de clase
    case 1:{
        timer.stop();
        isbastard = true;//nivel dificil
        hard = bastard();
        std::cout << "hard: "<<hard <<"\n";
        timer.start(timeoutTime(), this);
    }
    break;
    case 2:{
        dump.setRandomShape();//el dump se usa para acceder
        //al metodo setRandomShape, y poder elegir
        //al azar la pieza
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
    //la Forma de la Pieza Hard se le da una TetrizPiece
    pieza.setShape(hard);

    curPiece = pieza;//copia los valores del nextPiece al curPiece


    //este es la conversion de coordenadas para ver que posicion estamos
    //por defecto empieza de la corrdenada mas a la
    //izquierda superio derecha
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.minY();

    if (!tryMove(curPiece, curX, curY)) {
        curPiece.setShape(NoShape);
        timer.stop();
        comienzo = false;
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
    //QPaintDevice('Padre) <- QPixmap(Hijo)
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
    /*
    ¿Como ahcemos para que nuestras piezas cambien
        de color constantemente?

    al investigar en la documentacion,
    vimos que el metodo:
            void TetrixBoard::paintEvent(QPaintEvent *event)
    se ejcuta automaticamente una vez se comienza el juego
    para controlar este comportamiento
    se uso el TimerEvent
    QUe permite controlar este comportamiento

    Es por eso que al darnos cuenta de esto podemos hacer que cambie
    los colores de forma aleatoria
    ya que este metodo esta anidado en el metodo  "::paintEvent"
    */
    static constexpr QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };
    //hacemos un random del tablero de colores y logramos este comportamiento
    //ya que el tablero se renderiza cada vez que se mueva la pieza
    //con el metodo DropLineDown
    int random = QRandomGenerator::global()->bounded(7) + 1;

    //luego elgimos el color y se acabo
    QColor color = colorTable[random];
    painter.fillRect(x + 1,
                     y + 1,
                     squareWidth() -2,
                     squareHeight() -2,
                     color);

   // painter.setPen(color.black());
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
//aqui habia un metodo de debugging pero
//lo borramos por que este es la version final, esta en
//el github si desea verlo

//Este metodo Escanea el borde
// y retorna las coordenadas del borde en una
//lista simple de int coorde(miembro de la clase TetrixBoard)
void TetrixBoard::getCoordinate(int top_level){
    if(top_level == 1){
        return;
    }

    int cor = 0;
    /*
        Con la altura verdadera
        escanea hacia abajo
        hasta encontrar una coordenada
        hasta que la siguiente este ocupada
    */
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
    //como es un miembro de clase podemos acceder a el en la clase
    //y ya no retornar nada
}
/*
    Este metodo obtiene la altura verdadera
    en la exposiscion vimos como
    hay una altura Falsa, por que esa altura solo
    retorna una altura basada en cuantos bloques hay por columna
*/
int TetrixBoard::getTrueHeight(std::vector<int>& rows){
    //este metodo recibe un vector
    //con el numero de bloques en cada columna,
    //y de acuerdo a esp retorna la altura verdadera
    //en la sgte linea usamos la altura falsa
    auto max = std::max_element(rows.begin(),rows.end());
    int alturaMax = *max;
    //la sigte fila de la altura maxima debe estar llena de ceros
    //Y es basicamente lo que hace este algoritmo
    bool isTrueH = rowIsNoShape(alturaMax+1);
    if(isTrueH){//si la sgte linea esta llena de NoShape
        //significa que la altura verdara esta
        //a una fila mas de distancia
        return alturaMax+1;
    }else{
        //pero sino es asi
        //iteramos hasta hallarla
        int upper = 1;
        //mientras la fila no este llena de NoShape significa
        //que aun no se alcanzo la altura verdadera
        while(!rowIsNoShape(alturaMax+upper)){
            upper++;
        }
        //cuando la alcancemos la retornamos
        return alturaMax+upper;
    }
}
//este metodo solo verifica que todas
//celdas de una columna esten llena de NoShape
bool TetrixBoard::rowIsNoShape(int row){
    bool isNoShape = true;
    for(int j=0; j < BoardWidth; ++j){
        TetrixShape shape = shapeAt(j,row-1);
        if(shape != NoShape){
           isNoShape = false;
           break;//cortamos para que ya no pierda tiempo
        }
//        std::cout << int(shape);
    }
    return isNoShape;
}
//! [4]
void TetrixBoard::start() {
    //capturar el valor del metodo
    //este metodo es un SLOT
    //el cual se maneja en la clase TetrixWindow
    //para que se ejecute el usario debe rpesionar el boton "Start"
    if (isPaused)
        return;

    comienzo = true;
    waitSgteLine = false;
    numLinesRemoved = 0;
    numPiecesDropped = 0;
    score = 0;
    level = 1;
    clearBoard();
    //emit signal()
    //emitimos las señales que usaran los elots
    //de los numeros LCD
    //que usaran su eslot DISPLAY(int number)
    emit linesRemovedChanged(numLinesRemoved);
    emit scoreChanged(score);
    emit levelChanged(level);

    newPiece();//es la funcion donde se elige que metodo quiere
    timer.start(timeoutTime(), this);
}
//! [4]

//! [5]
void TetrixBoard::pause()
{
    //el pause hace el que el timerEvent haga
    //que se pare al painEvent()
    //printState(); //metodo de debugging
    if (!comienzo)
        return;
    isPaused = !isPaused;
    if (isPaused) {
        timer.stop();
    } else {
        timer.start(timeoutTime(), this);
    }
    //update();//
    //update();Es para pausar el estado del widget
//! [5] //! [6]
}
//! [6]

//! [7]
//!
//este metodo se ejecuta a cada instante
//debido al bucle que permite a
//la ventana seguir funcionando
void TetrixBoard::paintEvent(QPaintEvent *event) {
    //printState();// mala idea
    QFrame::paintEvent(event);//no entiendo
    QPainter painter(this);//
    QRect rect = contentsRect();
//! [7]
    if (isPaused) {

        painter.drawText(rect, Qt::AlignCenter, tr("Pause"));
        return;
    }

//! [8]
    int boardTop = rect.bottom() - BoardHeight*squareHeight();
    //este metodo dibuja un cuadrado cada en las coodenadas
    //especificadas
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
//  Si es != a NoShape entonces la dibujamos
//esto es debido a que hay un random que elige que pieza saldra
//por lo que para estar seguros este if esta aqui
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

    }

}

//Este metodo maneja las teclas a presionar para el tablero
//este metodo cuando tu le das a una flecha
//intenta contruir esa pieza en esas coordenadas
//pero sino puede retorna un False el TryMove()
void TetrixBoard::keyPressEvent(QKeyEvent *event)
{
    if (!comienzo || isPaused || curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
        case Qt::Key_Left:
            tryMove(curPiece, curX - 1, curY);
            break;
        case Qt::Key_Right:
            tryMove(curPiece, curX + 1, curY);
            break;
        case Qt::Key_Down:
            tryMove(curPiece.rotaR(), curX, curY);
            break;
        case Qt::Key_Up:
            tryMove(curPiece.rotaL(), curX, curY);
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

}
//este es el que conytrola la ejecucion del juego

void TetrixBoard::timerEvent(QTimerEvent *event)
{
    //debe ser el mismo ID
    if (event->timerId() == timer.timerId()) {
        if (waitSgteLine) {
            waitSgteLine = false;

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
// usa el metodo de trymove()
// para ver que todas encaje y cuando no encaje
// ya no se mueve la caja
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

}

void TetrixBoard::oneLineDown()// key D
{
    if (!tryMove(curPiece, curX, curY - 1))
       pieceDropped(0);
}

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

    if (!waitSgteLine)
    {

        //printState();
        newPiece();
    }
//! [22] //! [23]
}
//! [23]

//! [24]
//Este metodo remueve las lineas o filas
//que este llenas y != de NoShape's
void TetrixBoard::removeFullLines()
{
    int numFullLines = 0;

    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {
            //si alguno es NoShape,
            if (shapeAt(j, i) == NoShape) {
                // ya no es linea completa
                lineIsFull = false;
                break;
            }
        }
        //si la linea estaba llena
        if (lineIsFull) {
            //incrementamos el numero de lineas removidas
            ++numFullLines;
            //luego hacemos un traspaso de todo el tablero
            //una linea abajo
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j)
                    shapeAt(j, k) = shapeAt(j, k + 1);
            }
//! [25] //! [26]
            for (int j = 0; j < BoardWidth; ++j)
                shapeAt(j, BoardHeight - 1) = NoShape;
        }
    }
    //esta linea es para ver lo del score
    if (numFullLines > 0) {

        numLinesRemoved += numFullLines;
        score += 10 * numFullLines;
        //luego actualizamos nuestro score
        //de acuerdo al numero delineas removidas
        emit linesRemovedChanged(numLinesRemoved);
        emit scoreChanged(score);

        timer.start(500, this);
        waitSgteLine = true;
        curPiece.setShape(NoShape);
        update();
    }
}
//Este metodo es nuestro algoritmo de Bastard
//es un algoritmo que al principio fue dificil
//de implementar, pero luego se hizo un poco mas facil cada dia
TetrixShape TetrixBoard::bastard(){

    //aqui se usa la array coorde[](miembro de la clase)
    //dado que el la altura verdadera es un miembro
    //de esta clase
    //no necesitamos retornar nada
    getOfficeHeight();//tambien se encarga de actrualizar el array
    //con todos los bordes
    //ahora tenemos la altura verdadera

    //necesitamos un mapa que diga cuantos No encajes
    //hubo en cierta coordenada del borde
    //este mapa nos servira para esto
    std::map<TetrixShape,int> mapaHard ={
        {ZShape,0},//1
        { SShape,0},//2
        {LineShape,0},//3
        {TShape,0},//4
        {SquareShape,0},//5
        {LShape,0},//6
        {MirroredLShape,0}//7
    };
    //1...7
    //este bucle sirve para poder recorrer el array
    //con todos los bordes que recogimos del tablero
    for(int i=0; i < 19;i+=2 ){
        int x = coorde[i];//0,2,4,
        int y = BoardHeight - coorde[i+1];//este es una conversion
        //ya que el tablero funciona como un epejo invertido hacia arriba

        //luego hacemos una TetrixShape que empieze con
        //la 2da pieza dentro del enumTetrixShape
        TetrixShape forma = TetrixShape::ZShape;

        for(int j = int(forma); j < 8; ++j){//recorremos por shape
            forma = TetrixShape(j);//tenemos todas,L,S,T,Line,Square,etc ...
            TetrixPiece ficha;//esta ficha es la que se usa para encajar
            //en el borde
            ficha.setShape(forma);//le cambiamos su forma
            //lo que conlleva a tambn cambiarle sus coordenadas

            //este array almacenara todas
            //las posibilidades de rotacion en un punto en particular
            bool arr[4]={false};
            for(int k= 0; k<4; ++k ){
                arr[k] = tryMove(ficha,x,y);
                //----------
                ficha = ficha.rotaL();
            }
            //---ANALISIS
            //si es >= a 3 los falsos
            //lo elegimos
            //sino
            //false, true,true,true
            //
            //contamos el numero de falsos
            int count=0;
            for(int m = 0; m < 4; ++m){
                if(arr[m] == (!isbastard)){
                    count+=1;
                }
            }
            //luego vemos si es mayor o igual
            //si no se puede encajar en cualquiera de sus forma
            //entonces esa piezas seria la indicada
            //PERO SOLO para ese punto en particular
            //no para todo el borde
            if(count >= 4){
                mapaHard[forma]+=1;
                //ejemplo de recoleccion a la
                //4 trues ->facil ->significa 0 falses
                //3 trues -> +-  ->sifnifica 1 false
                //2 trues -> intermedio -> significa 2 falses
                //1 trues ->dificil -> que 3 falses muy dificil
            }
        }
    }
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
// cambia la variable por referencia max
// el valor a cambiar el el valor maximo de los vaores en los mapas
void TetrixBoard::getMaxOnMap(std::map<TetrixShape,int>& mapaHard,int&max){
    std::map<TetrixShape,int>::iterator iter;

    for (iter = mapaHard.begin(); iter != mapaHard.end(); iter++){
        std::cout <<"LLave: "<<iter->first<<" Vlaue: "<<iter->second<<"\n";
        if(max < iter->second){
            max = iter->second;

        }
    }
}
//borramos las pares de valores que sean
//menor al valor maximo de no encajadas en el borde
//para solo hacer un random entre los que estan mas dificiles
//a veces nos sale todas 8, asi que este metodo no borra nada
//pero si hay una de 9 y las demas 8, entonce
//solo deja un par en el mapa
void TetrixBoard::deleteShapesOnMap(std::map<TetrixShape,int>& mapaHard,int&max){
    std::map<TetrixShape,int>::iterator iter;
    //mayor dificultad //pero
    for(iter = mapaHard.begin(); iter != mapaHard.end(); iter++){
        if(iter->second < max){
            mapaHard.erase(iter->first);//1
        }
    }
}
//ahora que ya tenemos en el mapa ya filtrado
//por el anterior metodo
//solo no queda hace un random
//buscando un poco
//vimos que existe este el std::advance, que hace el random
//por nosotros
TetrixShape TetrixBoard::getRamdomHardPiece(std::map<TetrixShape,int>& mapaHard){
    srand(time(NULL));
//QRandomGenerator::global()->bounded(mapaHard.size()) + 1)
    std::map<TetrixShape,int>::iterator it = mapaHard.begin();
    //le damos in iterator y le da el puntero con un par
    //random del mapaHard
    std::advance(it,rand()%mapaHard.size());

    TetrixShape ramdon_key = TetrixShape(it->first);
    //luego retornamos
    return ramdon_key;
}
// DIFICIL

//funcion
// FACIL



//este metodo es el que no bota True
//si es que se puede mover


bool TetrixBoard::tryMove(const TetrixPiece &newPiece, int newX, int newY)
{
    //std::cout << "newX: "<<newX <<"\tnewY: "<<newY<<std::endl;//para debugging

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

