//  Created by Nathan Greenfield on 1/1/2020.
//  Copyright (c) 2020 Nathan Greenfield. All rights reserved.
#pragma once

struct GameState
{
	// state of each square in game
	enum SquareState { Empty, X, O };
	// Grid of spaces
	SquareState mBoard[3][3];
};