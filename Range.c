/*
 * Range.c
 *
 *  Created on: 2013-03-24
 *      Author: danny
 */

#include "Range.h"

struct Range* initRange(int x, int y, int w, int h) {
	struct Range* this = (struct Range*)malloc(sizeof(struct Range));
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
	return this;
}

void compareRange(struct Range* r1, struct Range* r2, int type){
	int r2_xRange_start = 4 * r2->x; // this is text button

	int r2_xRange_end = 4*r2->x + r2->width;
	if (r2_xRange_end > 320){
		r2_xRange_end = 320;
	}

	int r2_yRange_start = 4*r2->y;

	int r2_yRange_end = 4*r2->y + r2->height;
	if (r2_yRange_end > 240){
		r2_yRange_end = 240;
	}

	if (r1->x >= r2_xRange_start && r1->x < r2_xRange_end &&
		r1->y >= r2_yRange_start && r1->y < r2_yRange_end){
		//r1->currentlyCollided = 1;
		r2->currentlyCollided = 1;
	} else {
		r2->currentlyCollided = 0;
	}

	if (r2->previouslyCollided == 0 && r2->currentlyCollided == 1){
		printf("Collision! Calling collide() for type %d\n", type);
	}

	r2->previouslyCollided = r2->currentlyCollided;
}

/**
 * checkTextCollisionForMouse
 */
void checkTxtCollisionForMouse(void* cursor, void* button){
	struct Range* r_button = ((struct Range*)((struct Button*)button)->range);

	int x_button_start = 4 * r_button->x;
	int y_button_start = 4 * r_button->y - 3;
	int x_button_end = 4 * r_button->x + r_button->width;
	int y_button_end = 4 * r_button->y + r_button->height-3;
	if (x_button_end > 320) {x_button_end = 320;}
	if (y_button_end > 240) {y_button_end = 240;}

	if (((struct Cursor*)cursor)->super->r->x >= x_button_start && ((struct Cursor*)cursor)->super->r->x < x_button_end &&
				((struct Cursor*)cursor)->super->r->y >= y_button_start && ((struct Cursor*)cursor)->super->r->y < y_button_end){
			if (((struct Cursor*)cursor)->isLeftPressed) {
				if (((struct Button*)button)->isClicked == 2) {
					// button is not clicked, user just clicked somewhere, not on the button
					r_button->currentlyCollided = 0;
					((struct Button*)button)->isClicked = 0;
					((struct Cursor*)cursor)->isLeftPressed = false;
				} else if (((struct Button*)button)->isClicked == 0) {
					// button is clicked!
					r_button->currentlyCollided = 1;
					((struct Button*)button)->isClicked = 1;
				}
			} else {
				((struct Button*)button)->isClicked = 0;
				r_button->currentlyCollided = 0;
			}
		} else {
			// cursor does not overlap with this button,
			// set isClicked status to state 2.
			((struct Button*)button)->isClicked = 2;
			r_button->currentlyCollided = 0;
		}

	// this state will only happen once.
	if (r_button->previouslyCollided == 0 && r_button->currentlyCollided == 1){
		if (((struct Button*)button)->isClicked == 1){
			printf("Collision detected\n");
			((struct Button*)button)->collide(((struct Button*)button));
			((struct Cursor*)cursor)->isLeftPressed = 0;
		}
	}

	r_button->previouslyCollided = r_button->currentlyCollided;

	r_button = NULL;

}

void checkImgCollisionForMouse(void* cursor, void* button){
		struct Range* r_button = (struct Range*)((struct Button*)button)->range;

		int x_button_start = r_button->x;
		int y_button_start = r_button->y;
		int x_button_end = r_button->x + r_button->width;
		int y_button_end = r_button->y + r_button->height;
		if (x_button_end > 320) {x_button_end = 320;}
		if (y_button_end > 240) {y_button_end = 240;}

		if (((struct Cursor*)cursor)->super->r->x >= x_button_start && ((struct Cursor*)cursor)->super->r->x < x_button_end &&
			((struct Cursor*)cursor)->super->r->y >= y_button_start && ((struct Cursor*)cursor)->super->r->y < y_button_end){
				if (((struct Cursor*)cursor)->isLeftPressed) {
					if (((struct Button*)button)->isClicked == 2) {
						// button is not clicked, user just clicked somewhere, not on the button
						r_button->currentlyCollided = 0;
						((struct Button*)button)->isClicked = 0;
						((struct Cursor*)cursor)->isLeftPressed = false;
					} else if (((struct Button*)button)->isClicked == 0) {
						// button is clicked!
						r_button->currentlyCollided = 1;
						((struct Button*)button)->isClicked = 1;
					}
				} else {
					((struct Button*)button)->isClicked = 0;
					r_button->currentlyCollided = 0;
				}
		} else {
			// cursor does not overlap with this button,
			// set isClicked status to state 2.
			((struct Button*)button)->isClicked = 2;
			r_button->currentlyCollided = 0;
		}

		// this state will only happen once.
		if (r_button->previouslyCollided == 0 && r_button->currentlyCollided == 1){
			if (((struct Button*)button)->isClicked == 1){
				printf("Collision detected\n");
				((struct Button*)button)->collide(((struct Button*)button));
				((struct Cursor*)cursor)->isLeftPressed = 0;
			}
		}

		r_button->previouslyCollided = r_button->currentlyCollided;

		r_button = NULL;
}

void checkButtonCollision(void* c, void* mf){

	struct Cursor* cursor = (struct Cursor*)c;
	struct Frame* mainFrame = (struct Frame*)mf;

	// check menu buttons
	checkTxtCollisionForMouse(cursor, mainFrame->elements[0]->buttons[1]);
	checkTxtCollisionForMouse(cursor, mainFrame->elements[0]->buttons[0]);

	// check action buttons
	//checkImgCollisionForMouse(cursor, mainFrame->elements[1]->buttons[0]);
	//checkImgCollisionForMouse(cursor, mainFrame->elements[1]->buttons[1]);
	//checkImgCollisionForMouse(cursor, mainFrame->elements[1]->buttons[2]);
	//checkImgCollisionForMouse(cursor, mainFrame->elements[1]->buttons[3]);


	// check song buttons
	if (mainFrame->currentPanel == 0){
		int i = 0;
		for (i = 1; i <= mainFrame->elements[2]->button_size; i++){
			checkTxtCollisionForMouse(cursor, mainFrame->elements[2]->buttons[i]);
		}
	} else {
		int i = 0;
		for (i = 1; i <= mainFrame->elements[3]->button_size; i++){
			checkTxtCollisionForMouse(cursor, mainFrame->elements[3]->buttons[i]);
		}
	}

}


