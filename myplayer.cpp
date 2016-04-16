#include "myplayer.h"
#include <iostream>
#include <QDebug>
#include <math.h>
#include <stack>
using namespace std;


#define BUFFER 30
double xFrog = 0;
double yFrog = 0;
glm::vec2 frog;
//global roadmap for each light
QList<stack<glm::vec2>> traceRoute;
glm::vec2 evasive(Wall wall, glm::vec2 dir, int index);
glm::vec2 goThrough(glm::vec2 start, glm::vec2 end, int index);
//copy from draw
//void drawWall(QPainter painter);
glm::vec2 spiralSearch(glm::vec2 ret);
QList<QList<Wall>> wallList2D;
QVector<glm::vec2> dirForLight;

//QPainter painter;


bool withInWall(glm::vec2 end);

float area2(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);

MyPlayer::MyPlayer()
{
    this->playerName = "My Player";
}

void setVirtualWalls(QList<Wall*> walls) {
    for(int i = 0; i < walls.size();i++) {
        QList<Wall> wallList;
        // to get vec of wall
        glm::vec2 wallDir = glm::normalize(walls[i]->point1 - walls[i]->point2);
        //to get prependicular vec of wall
        glm::vec2 prepDir(wallDir.y, -wallDir.x);
//        std::cout<<wallDir.x<<", "<<wallDir.y<<std::endl;

        Wall temp1 = Wall(walls[i]->point1 + (float)BUFFER*prepDir + wallDir * (float)BUFFER, walls[i]->point2 + (float)BUFFER*prepDir - wallDir * (float)BUFFER);
        Wall temp2 = Wall(walls[i]->point1 - (float)BUFFER*prepDir + wallDir * (float)BUFFER, walls[i]->point2 - (float)BUFFER*prepDir - wallDir * (float)BUFFER);

        Wall temp3 = Wall(walls[i]->point1 + (float)BUFFER*prepDir + wallDir * (float)BUFFER, walls[i]->point1 - (float)BUFFER*prepDir + wallDir * (float)BUFFER);
        Wall temp4 = Wall(walls[i]->point2 + (float)BUFFER*prepDir - wallDir * (float)BUFFER, walls[i]->point2 - (float)BUFFER*prepDir - wallDir * (float)BUFFER);

        wallList.append(temp1);
        wallList.append(temp2);
        wallList.append(temp3);
        wallList.append(temp4);
        wallList2D.append(wallList);
//        drawWall(painter);

//        std::cout << "wallList2d size:" << wallList2D.size() << std::endl;
     }
}


/*
 * This method is called once at the start of the game.
 * The parameter specifies the number of mosquitoes at board->at(x).at(y), or board[x][y]
 * The return value is the vector specifying the x/y coordinates of the frog.
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */
glm::vec2 MyPlayer::initializeFrog(QVector<QVector<int> >* board) {
    walls = this->walls;
    setVirtualWalls(walls);

    /*
     * This places the frog in the center.
     * But you can place it anywhere you like!
     */
    double xret = 250;
    double yret = 250;

    xFrog = xret;
    yFrog = yret;
    glm::vec2 ret(xFrog,yFrog);

    ret = spiralSearch(ret);
    stack<glm::vec2> milestone;
//    milestone.push(frog);
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


    traceRoute[0].push(ret1);
    //add last time moving dir for each light, so that we can check if vec(next milestone - curr) has changed direction
    //meaning we passed milestone.
    dirForLight.append(ret1-ret0);
    traceRoute[1].push(ret3);
    dirForLight.append(ret3-ret1);
    traceRoute[2].push(ret0);
    dirForLight.append(ret0-ret2);
    traceRoute[3].push(ret2);
    dirForLight.append(ret2-ret3);

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
            //need to handel if stack top one close to frog
            //to do
            //jump out if only frog in stack or nextOne not reach
            if (glm::length(nextOne - currPos)<=0.6 && glm::dot(nextOne-currPos,dirForLight[i])<0) {
                if(traceRoute[i].size()!=1){
                    traceRoute[i].pop();
                    nextOne = traceRoute[i].top();
                }
            }
            //go to next one
            glm::vec2 deltaVec = glm::normalize(nextOne - currPos);
            deltaVec = goThrough(currPos, currPos+deltaVec, i);
            this->lights[i]->moveTo(currPos.x+0.6*deltaVec.x,currPos.y+0.6*deltaVec.y);
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
    } else if(glm::dot(wallDir, temp)<0){
        temp = wall.point2;
    } else {
        //nothing changed
        return (glm::vec2)NULL;
    }
    traceRoute[index].push(temp);
    cout<<"stack"<<index<< " size: "<<traceRoute[index].size()<<endl;
    cout<<"top is : " <<traceRoute[index].top().x<<","<<traceRoute[index].top().y<<endl;

    ret = temp;
//    std::cout<<wallDir.x<<","<<wallDir.y<<std::endl;
    return ret;
}

//index used to update stack with that index
glm::vec2 goThrough(glm::vec2 start, glm::vec2 end, int index) {

    glm::vec2 ret = end - start;

    for (int i = 0;i < wallList2D.size(); i++) {
        for(int j = 0;j<wallList2D[i].size();j++) {
            if((wallList2D[i][j]).isInvalidMove(start,end)) {
//                std::cout<< wallList2D[i][j].point1.x <<","<< wallList2D[i][j].point1.y<< std::endl;
//                std::cout<< wallList2D[i][j].point2.x <<","<< wallList2D[i][j].point2.y<< std::endl;
                ret = evasive(wallList2D[i][j], end-start, index);
                //not changed
                if(ret == (glm::vec2)NULL) {
                    ret = end - start;
                }
                ret = glm::normalize(ret-start);
                if(withInWall(start+ret)){
//                    std::cout<<ret.x<<","<<ret.y<<std::endl;
                    continue;
                }

//                if(glm::length(goThrough(start, start+ret) - ret) <= 0.1) {
//                    ret = -ret;
//                }

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
//       std::cout<<"sum:"<<sum<<std::endl;
       sum += areaTwo;
//       std::cout<<"sum:"<<sum<<std::endl;
//       std::cout<<"area:"<<glm::length(wallList2D[i][0].point1-wallList2D[i][1].point1)*glm::length(wallList2D[i][1].point1-wallList2D[i][1].point2)<<std::endl;

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
    if(withInWall(ret)) {
        while(withInWall(ret)) {
            ret.x = centerX + angle*cos(angle);
            ret.y = centerY + angle*sin(angle);
            angle += 0.001;
//            std::cout << "coordinate x, y:" <<ret.x<<","<<ret.y<<std::endl;
        }
    }
    return ret;
}

//void drawWall2(QPainter *painter) {
//    QPen circlePen = QPen(Qt::yellow);
//    circlePen.setWidth(2);
//    painter->setPen(circlePen);
//    std::cout<<"debug3"<<std::endl;
//    for (int i = 0;i < wallList2D.size(); i++) {
//        for(int j = 0;j<wallList2D[i].size();j++) {
//            std::cout<<"debug"<<std::endl;
//            painter->drawLine(QPoint(wallList2D[i][j].point1.x, wallList2D[i][j].point1.y), QPoint(wallList2D[i][j].point2.x, wallList2D[i][j].point2.y));
//        }

//    }

//}

//void Helper::drawWall(QPainter *painter, float px, float py, float p1x, float p1y) {
//    circlePen = QPen(Qt::red);
//    circlePen.setWidth(2);
//    painter->setPen(circlePen);

//    painter->drawLine(QPointF(px, py), QPoint(p1x, p1y));
//}

