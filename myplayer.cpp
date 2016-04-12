#include "myplayer.h"
#include <iostream>
#include <QDebug>
#include <math.h>
double xFrog = 0;
double yFrog = 0;
glm::vec2 frog;
int step = 0;
int stepfor2 = 0;
MyPlayer::MyPlayer()
{
    this->playerName = "My Player";
}


/*
 * This method is called once at the start of the game.
 * The parameter specifies the number of mosquitoes at board->at(x).at(y), or board[x][y]
 * The return value is the vector specifying the x/y coordinates of the frog.
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */
glm::vec2 MyPlayer::initializeFrog(QVector<QVector<int> >* board) {

    /*
     * This places the frog in the center.
     * But you can place it anywhere you like!
     */
    double xret = 250;
    double yret = 250;
//    for(int x = 0; x < 500; x++){
//        for(int y = 0; y < 500; y++){
//            xret += (*board)[x][y]*x;
//            yret += (*board)[x][y]*y;
//        }
//    }
//    xret = xret/500;
//    yret = yret/500;

    xFrog = xret;
    yFrog = yret;
    glm::vec2 ret(xFrog,yFrog);
    frog = ret;
    return ret;
}


/*
 * This method is called once at the start of the game.
 * You should set the initial position of each of the four lights.
 * The parameter specifies the number of mosquitoes at board->at(x).at(y), or board[x][y]
 * You can change the colors if you'd like!
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */
void MyPlayer::initializeLights(QVector<QVector<int> >* board) {
    step = 0;
    stepfor2 = 0;
    /*
    // this prints out debugging information about the walls
    qDebug() << "initializeLights: number of walls " << this->walls.size();
    for (int i = 0; i < this->walls.size(); i++) {
        Wall* wall = this->walls.at(i);
        qDebug() << "there is a wall from (" << wall->point1.x << "," << wall->point1.y << ") to (" << wall->point2.x << "," << wall->point2.y << ")" << endl;
    }
    */

    /*
    // this prints out debugging information about the mosquitoes
    for (int x = 0; x < board->size(); x++) {
        for (int y = 0; y < board->at(x).size(); y++) {
            int numMosquitoes = board->at(x).at(y);
            if (numMosquitoes > 0)
              qDebug() << "There are " << numMosquitoes << " mosquitoes at (" << x << ", " << y << ")" << endl;
        }
    }
    */


    /*
     * You can, of course, put the lights anywhere you like!
     */
    double xSet = 70.7;
    double ySet = 70.7;
//    int count = 0;
//    for (int i = 0; i < 250 ; i++) {
//        for (int j = 0; j < 250; j++) {
//            xSet += (*board)[i][j]*i;
//            ySet += (*board)[i][j]*j;
//            count += (*board)[i][j];
//        }
//    }
//    xSet /= count;
//    ySet /= count;
    this->lights.at(0)->setInitialPosition(xSet, ySet);
    this->lights.at(0)->trailColor = QColor(255, 255, 255);

    xSet = 429.3;
    ySet = 70.7;
//    count = 0;
//    for (int i = 250; i < 500 ; i++) {
//        for (int j = 0; j < 250; j++) {
//            xSet += (*board)[i][j]*i;
//            ySet += (*board)[i][j]*j;
//            count += (*board)[i][j];
//        }
//    }
//    xSet /= count;
//    ySet /= count;
    this->lights.at(1)->setInitialPosition(xSet, ySet);
    this->lights.at(1)->trailColor = QColor(0, 255, 255);

    xSet = 70.7;
    ySet = 429.3;
//    count = 0;
//    for (int i = 0; i < 250 ; i++) {
//        for (int j = 250; j < 500; j++) {
//            xSet += (*board)[i][j]*i;
//            ySet += (*board)[i][j]*j;
//            count += (*board)[i][j];
//        }
//    }
//    xSet /= count;
//    ySet /= count;
    this->lights.at(2)->setInitialPosition(xSet, ySet);
    this->lights.at(2)->trailColor = QColor(255, 255, 0);

    xSet = 429.3;
    ySet = 470;
//    count = 0;
//    for (int i = 250; i < 500 ; i++) {
//        for (int j = 250; j < 500; j++) {
//            xSet += (*board)[i][j]*i;
//            ySet += (*board)[i][j]*j;
//            count += (*board)[i][j];
//        }
//    }
//    xSet /= count;
//    ySet /= count;
    this->lights.at(3)->setInitialPosition(xSet, ySet);
    this->lights.at(3)->trailColor = QColor(255, 0, 255);


}

//double[] MyPlayer::calcRange(int index) {
//    double range[4];
//    range[0] = (index % 2 == 0) ?  0 : 250;
//    range[1] = (index % 2 == 0) ? 250 : 500;
//    range[2] = (index > 1) ? 250 : 0;
//    range[3] = (index > 1) ? 500 : 250;
//    return ret;
//}

/*
 * This method is called once per "step" in the simulation.
 * You can change the position of each Light by calling its moveTo method with the new x/y coordinate.
 * You may not, however, move a Light by more than one unit, and you may not move it through a wall!
 * The parameter specifies the number of mosquitoes at board->at(x).at(y)
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */

void MyPlayer::updateLights(QVector<QVector<int> >* board) {

    int index = 0;
    // i = 0 ,1, 2
    if(step == 0) {
        for (int i = 0; i < this->lights.length() && i != 3; i++) {
            if(i==2) index = 3;
            else{
                index = i;
            }
            glm::vec2 currPos = this->lights.at(i)->getPosition();
            if (i == 0 && currPos.x > 330) step = 1;
            this->lights[i]->moveTo(currPos.x+0.6*cos(0.5*M_PI*index),currPos.y+0.6*sin(0.5*M_PI*index));
        }
    } else {
        for (int i = 0; i < this->lights.length() && i != 3; i++) {
            glm::vec2 currPos = this->lights[i]->getPosition();
            float length = glm::length(frog - currPos);
            glm::vec2 deltaVec = (frog - currPos)/length;
            this->lights[i]->moveTo(currPos.x+0.6*deltaVec.x,currPos.y+0.6*deltaVec.y);
        }
    }

    //i == 2 strategy this->light.at(3)
    if(stepfor2 == 0) {
        glm::vec2 currPos1 = this->lights.at(3)->getPosition();
        if (currPos1.x < 50) stepfor2 = 1;
        this->lights[3]->moveTo(currPos1.x+0.6*cos(0.5*M_PI*2),currPos1.y+0.6*sin(0.5*M_PI*2));
    } else if (stepfor2 == 1) {
        glm::vec2 currPos1 = this->lights.at(3)->getPosition();
        if (currPos1.y < 200) stepfor2 = 3;
        this->lights[3]->moveTo(currPos1.x+0.6*cos(0.5*M_PI*3),currPos1.y+0.6*sin(0.5*M_PI*3));
    } else {
        glm::vec2 currPos1 = this->lights[3]->getPosition();
        float length = glm::length(frog - currPos1);
        glm::vec2 deltaVec = (frog - currPos1)/length;
        this->lights[3]->moveTo(currPos1.x+0.6*deltaVec.x,currPos1.y+0.6*deltaVec.y);

    }

}





