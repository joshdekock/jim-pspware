/*
 * $Id$
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Shot data.
 *
 * @version $Revision$
 */
#include "vector.h"

typedef struct {
  Vector pos;
  int cnt;
} Shot;

#define SHOT_MAX 16

#define SHOT_SPEED 4096

#define SHOT_WIDTH 8
#define SHOT_HEIGHT 24

extern Shot shot[];

void initShots();
void moveShots();
void drawShots();
void addShot(Vector *pos);
