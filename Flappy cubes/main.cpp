#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define BIRD_SIZE 20
#define PIPE_WIDTH 40
#define PIPE_COUNT 5
#define PIPE_SPEED 6
#define PIPE_GAP 200
#define GRAVITY 1
#define SLEEPTIME 25

typedef struct {
    float birdX;
    float birdY;
    float birdVelocity;
    float pipeX[PIPE_COUNT];
    float pipeY[PIPE_COUNT];
    int gameover;
    int start;
    int score;
    clock_t startTime;
} GameState;

GameState state;
int menuState = 1;

void initGame() {
    state.birdX = 50;
    state.birdY = SCREEN_HEIGHT / 2;
    state.birdVelocity = 0;
    state.gameover = 0;
    state.start = 0;
    state.score = 0;
    state.startTime = clock();

    srand(time(NULL));
    for (int i = 0; i < PIPE_COUNT; i++) {
        state.pipeX[i] = SCREEN_WIDTH + i * (SCREEN_WIDTH / PIPE_COUNT) * 2;
        state.pipeY[i] = rand() % (SCREEN_HEIGHT - PIPE_GAP - BIRD_SIZE * 4);
    }
}

void drawBird() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(state.birdX, state.birdY);
    glVertex2f(state.birdX + BIRD_SIZE, state.birdY);
    glVertex2f(state.birdX + BIRD_SIZE, state.birdY + BIRD_SIZE);
    glVertex2f(state.birdX, state.birdY + BIRD_SIZE);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(state.birdX + BIRD_SIZE, state.birdY + BIRD_SIZE / 2);
    glVertex2f(state.birdX + BIRD_SIZE * 1.5, state.birdY + BIRD_SIZE);
    glVertex2f(state.birdX + BIRD_SIZE, state.birdY + BIRD_SIZE * 1.5);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(state.birdX, state.birdY + BIRD_SIZE / 2);
    glVertex2f(state.birdX - BIRD_SIZE * 0.5, state.birdY + BIRD_SIZE);
    glVertex2f(state.birdX, state.birdY + BIRD_SIZE * 1.5);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    float eyeRadius = BIRD_SIZE / 10.0f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1415926 / 180;
        float x = state.birdX + BIRD_SIZE * 0.3 + eyeRadius * cos(theta);
        float y = state.birdY + BIRD_SIZE * 0.7 + eyeRadius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1415926 / 180;
        float x = state.birdX + BIRD_SIZE * 0.7 + eyeRadius * cos(theta);
        float y = state.birdY + BIRD_SIZE * 0.7 + eyeRadius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawPipe(float x, float y, float height) {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + PIPE_WIDTH, y);
    glVertex2f(x + PIPE_WIDTH, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawPipes() {
    for (int i = 0; i < PIPE_COUNT; i++) {
        drawPipe(state.pipeX[i], 0, state.pipeY[i]);
        drawPipe(state.pipeX[i], state.pipeY[i] + PIPE_GAP, SCREEN_HEIGHT - state.pipeY[i] - PIPE_GAP);
    }
}

void checkCollision() {
    for (int i = 0; i < PIPE_COUNT; i++) {
        if (state.birdX + BIRD_SIZE >= state.pipeX[i] && state.birdX <= state.pipeX[i] + PIPE_WIDTH &&
            (state.birdY <= state.pipeY[i] || state.birdY + BIRD_SIZE >= state.pipeY[i] + PIPE_GAP)) {
            state.gameover = 1;
        }
    }

    if (state.birdY <= 0 || state.birdY + BIRD_SIZE >= SCREEN_HEIGHT) {
        state.gameover = 1;
    }
}

void movePipes() {
    for (int i = 0; i < PIPE_COUNT; i++) {
        state.pipeX[i] -= PIPE_SPEED;
        if (state.pipeX[i] < -PIPE_WIDTH) {
            state.pipeX[i] = SCREEN_WIDTH;
            state.pipeY[i] = rand() % (SCREEN_HEIGHT - PIPE_GAP - BIRD_SIZE * 4);
            state.score++;
        }
    }
}

void moveBird() {
    state.birdVelocity -= GRAVITY;
    state.birdY += state.birdVelocity;

    if (state.birdY < 0) {
        state.birdY = 0;
        state.birdVelocity = 0;
    } else if (state.birdY + BIRD_SIZE > SCREEN_HEIGHT) {
        state.birdY = SCREEN_HEIGHT - BIRD_SIZE;
        state.birdVelocity = 0;
    }
}
void drawCube(float x, float y, float size) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
}

void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.8f, 1.0f);
    glVertex2f(0, SCREEN_HEIGHT);
    glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glColor3f(0.8f, 0.95f, 1.0f);
    glVertex2f(SCREEN_WIDTH, 0);
    glVertex2f(0, 0);
    glEnd();


    glColor3f(0.8f, 0.6f, 0.4f);
    for (int i = 0; i < SCREEN_WIDTH; i += 40) {
        for (int j = 0; j < SCREEN_HEIGHT; j += 40) {
            drawCube(i, j, 20);
        }
    }
}



void drawButton(float x, float y, float width, float height, const char* text) {
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + width / 4, y + height / 2);
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void drawMenu() {
    drawBackground();

    drawButton(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, 50, "Start Game");
    drawButton(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 - 100, SCREEN_WIDTH / 2, 50, "Quit");
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (menuState == 1) {
        drawMenu();
    } else {
        drawBackground();

        if (!state.start) {
            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2);
            char menuStr[] = "Press Space to Start";
            int len = strlen(menuStr);
            for (int i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, menuStr[i]);
            }
        } else if (!state.gameover) {
            moveBird();
            drawBird();
            drawPipes();

            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(10, SCREEN_HEIGHT - 20);
            char scoreStr[50];
            sprintf(scoreStr, "Score: %d", state.score);
            int len = strlen(scoreStr);
            for (int i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
            }

            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 20);
            double elapsed = (double)(clock() - state.startTime) / CLOCKS_PER_SEC;
            char timerStr[50];
            sprintf(timerStr, "Timer: %.1f s", elapsed);
            len = strlen(timerStr);
            for (int i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timerStr[i]);
            }
        } else {
            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2);
            char finalScoreStr[50];
            sprintf(finalScoreStr, "Final Score: %d", state.score);
            int len = strlen(finalScoreStr);
            for (int i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, finalScoreStr[i]);
            }
        }
    }

    glutSwapBuffers();
}

void moveScreen(void) {
    if (menuState == 2 && state.start && !state.gameover) {
        movePipes();
        checkCollision();
        glutPostRedisplay();
    }
    Sleep(SLEEPTIME);
}

void keyboard(unsigned char key, int x, int y) {
    if (menuState == 2) {
        if (key == ' ' && !state.gameover) {
            state.start = 1;
            state.birdVelocity = 10;
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (menuState == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mx = (float)x / glutGet(GLUT_WINDOW_WIDTH) * SCREEN_WIDTH;
        float my = SCREEN_HEIGHT - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * SCREEN_HEIGHT;
        if (mx >= SCREEN_WIDTH / 4 && mx <= SCREEN_WIDTH * 3 / 4) {
            if (my >= SCREEN_HEIGHT / 2 && my <= SCREEN_HEIGHT / 2 + 50) {
                menuState = 2;
                initGame();
                glutPostRedisplay();
            } else if (my >= SCREEN_HEIGHT / 2 - 100 && my <= SCREEN_HEIGHT / 2 - 50) {
                exit(0);
            }
        }
    }
}

void myInit() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Flappy Cubes");

    initGame();

    glutDisplayFunc(myDisplay);
    glutIdleFunc(moveScreen);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    myInit();
    glutMainLoop();

    return 0;
}
