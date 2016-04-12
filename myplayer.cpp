#include "myplayer.h"
#include <QDebug>

bool caught1 = false;
bool caught2 = false;
bool caught3 = false;
bool caught4 = false;

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
    return glm::vec2(250.0f, 250.0f);
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
    this->lights.at(0)->setInitialPosition(69.0f, 69.0f);
    this->lights.at(0)->trailColor = QColor(255, 255, 255);

    this->lights.at(1)->setInitialPosition(431.0f, 69.0f);
    this->lights.at(1)->trailColor = QColor(0, 255, 255);

    this->lights.at(2)->setInitialPosition(69.0f, 431.0f);
    this->lights.at(2)->trailColor = QColor(255, 255, 0);

    this->lights.at(3)->setInitialPosition(431.0f, 431.0f);
    this->lights.at(3)->trailColor = QColor(255, 0, 255);

}



/*
 * This method is called once per "step" in the simulation.
 * You can change the position of each Light by calling its moveTo method with the new x/y coordinate.
 * You may not, however, move a Light by more than one unit, and you may not move it through a wall!
 * The parameter specifies the number of mosquitoes at board->at(x).at(y)
 * You can access the walls through this object's "walls" field, which is a vector of Wall*
 */

void MyPlayer::updateLights(QVector<QVector<int> >* board) {


    for (int j = 0; j < this->lights.length(); j++) {

        // this gets the current position of the light
        glm::vec2 currPos = this->lights.at(j)->getPosition();

        /*
         * This is a pretty bad solution!
         * For part 1, modify this code so that the lights bring all the mosquitoes
         * to the frog within 5000 steps.
         */
        if (j == 0) {
            if (!caught1 && currPos.y < 300.0 && currPos.x >= 69.0) {
                this->lights.at(j)->moveTo(69.0, currPos.y + 0.577);
            } else if (currPos.y >= 300.0 && currPos.x < 250.0) {
                this->lights.at(j)->moveTo(currPos.x + 0.577, 300.0);
                caught1 = true;
            } else if (caught1 && currPos.y >= 250.0 && currPos.x >= 250.0) {
                this->lights.at(j)->moveTo(250.0, currPos.y - 0.577);
            } else if (caught1 && currPos.y <= 250.0 && currPos.x == 250.0) {
                this->lights.at(j)->moveTo(250.0, 250.0);
                caught1 = false;
            }
        }
        else if (j == 1) {
            if (!caught2 && currPos.y <= 69.0 && currPos.x > 200) {
                this->lights.at(j)->moveTo(currPos.x - 0.577, 69.0);
            } else if (currPos.y < 250.0 && currPos.x <= 200.0) {
                this->lights.at(j)->moveTo(200.0, currPos.y + 0.577);
                caught2 = true;
            } else if (caught2 && currPos.y >= 250.0 && currPos.x <= 250.0) {
                this->lights.at(j)->moveTo(currPos.x + 0.577, 250.0);
            } else if (caught2 && currPos.y == 250.0 && currPos.x >= 250.0) {
                this->lights.at(j)->moveTo(250.0, 250.0);
                caught2 = false;
            }
        }
        else if (j == 2) {
            if (!caught3 && currPos.y >= 431.0 && currPos.x < 300) {
                this->lights.at(j)->moveTo(currPos.x + 0.577, 431.0);
            } else if (currPos.y > 250.0 && currPos.x >= 300.0) {
                this->lights.at(j)->moveTo(300.0, currPos.y - 0.577);
                caught3 = true;
            } else if (caught3 && currPos.y <= 250.0 && currPos.x >= 250.0) {
                this->lights.at(j)->moveTo(currPos.x - 0.577, 250.0);
            } else if (caught3 && currPos.y == 250.0 && currPos.x <= 250.0) {
                this->lights.at(j)->moveTo(250.0, 250.0);
                caught3 = false;
            }
        }
        else if (j == 3) {
            if (!caught4 && currPos.y > 200.0 && currPos.x >= 431.0) {
                this->lights.at(j)->moveTo(431.0, currPos.y - 0.577);
            } else if (currPos.y <= 200.0 && currPos.x > 250.0) {
                this->lights.at(j)->moveTo(currPos.x - 0.577, 200.0);
                caught4 = true;
            } else if (caught4 && currPos.y <= 250.0 && currPos.x <= 250.0) {
                this->lights.at(j)->moveTo(250.0, currPos.y + 0.577);
            } else if (caught4 && currPos.y >= 250.0 && currPos.x == 250.0) {
                this->lights.at(j)->moveTo(250.0, 250.0);
                caught4 = false;
            }
        }


    }



}

