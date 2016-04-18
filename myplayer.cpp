#include "myplayer.h"
#include <iostream>
#include <QDebug>
#include <math.h>
#include <stack>
#include <QPainter>
#include "helper.h"
using namespace std;


double buffer = 30;
double xFrog = 0;
double yFrog = 0;
glm::vec2 frog;
//global roadmap for each light
QList<stack<glm::vec2>> traceRoute;
glm::vec2 evasive(Wall wall, glm::vec2 dir, int index);
glm::vec2 goThrough(glm::vec2 start, glm::vec2 end, int index);
glm::vec2 spiralSearch(glm::vec2 ret);
QList<QList<Wall>> wallList2D;
QVector<glm::vec2> dirForLight;
QVector<glm::vec2> lastPop;
//QVector<glm::vec2> lastDir;

//rewrite isInvalidMove used on Virtual Wall cases, which allow colinear moves and cross endpoints of lines
bool isInvalidMoveForVirtualWall(glm::vec2 start, glm::vec2 end, Wall wall);
//this function is used to determine if vec is within virtual walls, excluding cases on the wall eades
bool withInWall(glm::vec2 end);
//cross product of vec2(p2-p1), vec2(p3-p1), clac the two times of area of triangle p1p2p3.
float area2(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
glm::vec2 adjustCoordinate(glm::vec2 point);

MyPlayer::MyPlayer()
{
    this->playerName = "My Player";
}

void setVirtualWalls(QList<Wall*> walls, int option) {
    //reset virtual walls distance
    wallList2D.clear();
    for(int i = 0; i < walls.size();i++) {
        QList<Wall> wallList;
        // to get vec of wall
        glm::vec2 wallDir = glm::normalize(walls[i]->point1 - walls[i]->point2);
        //to get prependicular vec of wall
        glm::vec2 prepDir(wallDir.y, -wallDir.x);
//        std::cout<<wallDir.x<<", "<<wallDir.y<<std::endl;
        glm::vec2 p11 = glm::vec2(walls[i]->point1 + (float)buffer*prepDir + wallDir * (float)buffer);
        glm::vec2 p21 = glm::vec2(walls[i]->point2 + (float)buffer*prepDir - wallDir * (float)buffer);
        glm::vec2 p12 = glm::vec2(walls[i]->point1 - (float)buffer*prepDir + wallDir * (float)buffer);
        glm::vec2 p22 = glm::vec2(walls[i]->point2 - (float)buffer*prepDir - wallDir * (float)buffer);

        if (option == 1) {
            p11 = adjustCoordinate(p11);
            p21 = adjustCoordinate(p21);
            p12 = adjustCoordinate(p12);
            p22 = adjustCoordinate(p22);
        }

        Wall temp1 = Wall(p11, p21);
        Wall temp2 = Wall(p12, p22);
        Wall temp3 = Wall(p11, p12);
        Wall temp4 = Wall(p21, p22);

        wallList.append(temp1);
        wallList.append(temp2);
        wallList.append(temp3);
        wallList.append(temp4);
        wallList2D.append(wallList);
     }
}

/* helper function to adjust the possible out of board milestone */
glm::vec2 adjustCoordinate(glm::vec2 point) {
    glm::vec2 ret = glm::vec2(point.x, point.y);
    if (ret.x <= 0) {
        ret.x = 0.5;
        cout << "changed" << point.x << endl;
    } else if (ret.x >= 500) {
        ret.x = 499.5;
        cout << "changed" << point.x << endl;
    }
    if (ret.y <= 0) {
        ret.y = 0.5;
        cout << "changed" << point.y << endl;
    } else if (ret.y >= 500) {
        ret.y = 499.5;
        cout << "changed" << point.y << endl;
    }
    return ret;
}

/*
 * This method is called once at the start of the game.
 * The parameter specifies the number of mosquitoes at board->at(x).at(y), or board[x][y]
 * The return value is the vector specifying the x/y coordinates of the frog.
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */
glm::vec2 MyPlayer::initializeFrog(QVector<QVector<int> >* board) {
    buffer = 80;
    walls = this->walls;
    setVirtualWalls(walls, 0);

    /*
     * This places the frog in the center.
     * But you can place it anywhere you like!
     */
    double xret = 250;
    double yret = 250;

    xFrog = xret;
    yFrog = yret;
    glm::vec2 ret(xFrog,yFrog);

    //runing a spiral search of frog position
    ret = spiralSearch(ret);
    //stack for miletones
    stack<glm::vec2> milestone;
    milestone.push(ret);
    traceRoute.append(milestone);
    traceRoute.append(milestone);
    traceRoute.append(milestone);
    traceRoute.append(milestone);
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

    //reset buffer to 50
    buffer = 50;
    setVirtualWalls(walls, 0);
    double xSet = 70.7;
    double ySet = 70.7;
    glm::vec2 ret0(xSet, ySet);
    ret0 = spiralSearch(ret0);

    this->lights.at(0)->setInitialPosition(ret0.x, ret0.y);
    this->lights.at(0)->trailColor = QColor(255, 255, 255);


    xSet = 429.3;
    ySet = 70.7;
    glm::vec2 ret1(xSet, ySet);
    ret1 = spiralSearch(ret1);
    this->lights.at(1)->setInitialPosition(ret1.x, ret1.y);
    this->lights.at(1)->trailColor = QColor(0, 255, 255);



    xSet = 70.7;
    ySet = 429.3;

    glm::vec2 ret2(xSet, ySet);
    ret2 = spiralSearch(ret2);
    this->lights.at(2)->setInitialPosition(ret2.x, ret2.y);
    this->lights.at(2)->trailColor = QColor(255, 255, 0);

    xSet = 429.3;
    ySet = 429.3;

    glm::vec2 ret3(xSet, ySet);
    ret3 = spiralSearch(ret3);
    this->lights.at(3)->setInitialPosition(ret3.x, ret3.y);
    this->lights.at(3)->trailColor = QColor(255, 0, 255);



    //push milestones for each lights.
    traceRoute[0].push(ret1);
    // for each light, add moving dir of last time to global variable dirForLight, so that we can check if vec(next milestone - currPos) in updateLight method has changed direction
    //meaning we passed milestone.
    dirForLight.append(glm::normalize(ret1-ret0));
    traceRoute[1].push(ret3);
    dirForLight.append(glm::normalize(ret3-ret1));
    traceRoute[2].push(ret0);
    dirForLight.append(glm::normalize(ret0-ret2));
    traceRoute[3].push(ret2);
    dirForLight.append(glm::normalize(ret2-ret3));

    for (int i = 0; i < 4; i++) {
        lastPop.append(glm::vec2(-1, -1));
//        lastDir.append(glm::vec2(5, 5));
    }

    //reset virtual walls
    buffer = 35;
    setVirtualWalls(walls, 1);

}



/*
 * This method is called once per "step" in the simulation.
 * You can change the position of each Light by calling its moveTo method with the new x/y coordinate.
 * You may not, however, move a Light by more than one unit, and you may not move it through a wall!
 * The parameter specifies the number of mosquitoes at board->at(x).at(y)
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */

void MyPlayer::updateLights(QVector<QVector<int> >* board) {
    for (int i = 0; i < this->lights.length(); i++) {
        if(traceRoute[i].empty()) {
            //deal with to do followed
            //edge case if not caught 90%
        } else {

            glm::vec2 currPos = this->lights[i]->getPosition();
            glm::vec2 nextOne = traceRoute[i].top();

            // determine if should pop out & update nextOne
            if (glm::length(nextOne - currPos) <= 0.6 && glm::dot(nextOne - (currPos + (float) 0.6 * dirForLight[i]), dirForLight[i]) < 0) {
//                int mosNum = 0;
//                for (int x = 0; x < board->size(); x++) {
//                    for (int y = 0; y < board->at(x).size(); y++) {
//                        int numMosquitoes = board->at(x).at(y);
//                        mosNum += numMosquitoes;
//                    }
//                }
//                if (mosNum >= 480) {
//                    glm::vec2 frog(xFrog,yFrog);
//                    stack<glm::vec2> headToFrog;
//                    headToFrog.push(frog);
//                    traceRoute.clear();
//                    for (int i = 0; i < 4; i++) {
//                        traceRoute.append(headToFrog);
//                    }
//                } else if
                 if (traceRoute[i].size() != 1){
                    glm::vec2 temp = traceRoute[i].top();
                    cout << "poped from " << i << ":" << temp.x <<","<<temp.y<<endl;
                    lastPop[i] = temp;
//                    lastDir[i] = dirForLight[i];

                    traceRoute[i].pop();
                    nextOne = traceRoute[i].top();
                    cout << i << " heading to " << nextOne.x <<","<<nextOne.y<<endl;
                }
            }

            //go to next one
            glm::vec2 deltaVec = glm::normalize(nextOne - currPos);
            deltaVec = goThrough(currPos, currPos + deltaVec, i);
            this->lights[i]->moveTo(currPos.x + 0.6 * deltaVec.x, currPos.y + 0.6 * deltaVec.y);
            dirForLight[i] = deltaVec;
        }
    }
}

// refract needed
//to do
//return next milestone
glm::vec2 evasive(Wall wall, glm::vec2 dir, int index) {
    glm::vec2 ret;
    glm::vec2 wallDir = glm::normalize(wall.point1 - wall.point2);
    glm::vec2 temp(-dir.y,dir.x);

    if(glm::dot(wallDir, temp)>0) {
        temp = wall.point1;
    } else if(glm::dot(wallDir, temp)<0) {
        temp = wall.point2;
    } else {
        //nothing changed, don't push, will not happen
        return (glm::vec2)NULL;
    }

    //to check if push the duplicate, only different milestone will be pushed
    if(glm::length(traceRoute[index].top()-temp)>0.0001 && glm::length(lastPop[index] - temp) > 0.0001){
//            && (lastDir[index].x * lastDir[index].y * dirForLight[index].x * dirForLight[index].y < 1
//                && abs(lastDir[index].x) - abs(dirForLight[index].x) < 0.005
//                && abs(lastDir[index].y) - abs(dirForLight[index].y) < 0.005)) {
        traceRoute[index].push(temp);
        cout << "pushed to " << index <<":" << temp.x <<","<<temp.y << endl;
//        cout << "lastDir of  " << index <<":" << lastDir[index].x <<","<<lastDir[index].y << endl;
//        cout << "curDir of  " << index <<":" << dirForLight[index].x <<","<<dirForLight[index].y << endl;
    }

    ret = temp;
    return ret;
}

//index used to update stack with that index
glm::vec2 goThrough(glm::vec2 start, glm::vec2 end, int index) {

    glm::vec2 ret = end - start;

    for (int i = 0;i < wallList2D.size(); i++) {
        for(int j = 0;j<wallList2D[i].size();j++) {
            //only invalid move && end point without virtual walls can be count as invalid move
            //rule out cases about crossing common point for two adjacent walls
            if(isInvalidMoveForVirtualWall(start,end,wallList2D[i][j]) && withInWall(end)) {
                cout<<"Invalid!"<<endl;
                std::cout<< i << "," << j << "," << wallList2D[i][j].point1.x <<","<< wallList2D[i][j].point1.y<< std::endl;
                std::cout<< i << "," << j << "," << wallList2D[i][j].point2.x <<","<< wallList2D[i][j].point2.y<< std::endl;
                ret = evasive(wallList2D[i][j], end-start, index);
                //not changed
                if(ret == (glm::vec2)NULL) {
                    ret = end - start;
                }
                ret = glm::normalize(ret-start);
                return ret;
            }

        }
    }
    return ret;
}

//to test if light is within wall
bool withInWall(glm::vec2 end) {
    for (int i = 0;i < wallList2D.size(); i++) {

       float sum =0;
       float areaOne = abs(area2(end.x, end.y, wallList2D[i][0].point1.x, wallList2D[i][0].point1.y,wallList2D[i][0].point2.x,wallList2D[i][0].point2.y));
       float areaTwo = abs(area2(end.x, end.y, wallList2D[i][1].point1.x, wallList2D[i][1].point1.y,wallList2D[i][1].point2.x,wallList2D[i][1].point2.y));
       float areaThree = abs(area2(end.x, end.y, wallList2D[i][2].point1.x, wallList2D[i][2].point1.y,wallList2D[i][2].point2.x,wallList2D[i][2].point2.y));
       float areaFour = abs(area2(end.x, end.y, wallList2D[i][3].point1.x, wallList2D[i][3].point1.y,wallList2D[i][3].point2.x,wallList2D[i][3].point2.y));
       sum += areaOne;
       sum += areaTwo;
       if(areaOne!=0.0f && areaTwo!=0.0f && areaThree!=0.0f && areaFour!=0.0f &&(abs(sum-(glm::length(wallList2D[i][0].point1-wallList2D[i][1].point1)*glm::length(wallList2D[i][1].point1-wallList2D[i][1].point2)) <= 1))) {
           return true;
        }
    }
    return false;
}


glm::vec2 spiralSearch(glm::vec2 ret) {
    double angle = 0;
    double centerX = ret.x;
    double centerY = ret.y;
    while(withInWall(ret)) {
        ret.x = centerX + angle*cos(angle);
        ret.y = centerY + angle*sin(angle);
        angle += 0.001;
    }
    return ret;
}

//rewrite isInvalidMove used on Virtual Wall cases, which allow colinear moves and cross endpoints of lines
bool isInvalidMoveForVirtualWall(glm::vec2 start, glm::vec2 end, Wall wall)
{

    float x1 = start.x;
    float y1 = start.y;
    float x2 = end.x;
    float y2 = end.y;

    float x3 = wall.point1.x;
    float y3 = wall.point1.y;
    float x4 = wall.point2.x;
    float y4 = wall.point2.y;
    float a1, a2, a3, a4;

    a1 = area2(x1, y1, x2, y2, x3, y3);
    a2 = area2(x1, y1, x2, y2, x4, y4);
    a3 = area2(x3, y3, x4, y4, x1, y1);
    a4 = area2(x3, y3, x4, y4, x2, y2);
    if(a1==0.0f||a2==0.0f||a3==0.0f||a4==0.0f) {
        return false;
    }
    return ((a1 > 0.0) ^ (a2 > 0.0)) && ((a3 > 0.0) ^ (a4 > 0.0));

}
