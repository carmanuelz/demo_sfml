--animation arraydata: nro frame, width, height, nrorow
mob001 = {
	type = 4,
	src = "assets/images/characters/enemmy.png",
	start = {5, 70, 44, 2},
	idle = {8, 70, 44, 1},
	run = {4, 70, 44, 0},
	attack  = {4, 70, 44, 3},
	offsetXR = -47,
	offsetXL = -23,
	offsetYR = -28,
	offsetYL = -28,
	hasweapon = false,
	damage = 5,
	HP = 40,
	weapon  = "",
}

mob002 = {
	type = 4,
	src = "assets/images/characters/player2.png",
	run = {8, 48, 60, 0},
	idle = {8, 48, 60, 1},	
	dead = {9, 72, 60, 2},
	attack = {8, 48, 60, 0},	
	offsetXR = -24,
	offsetXL = -24,
	offsetYR = -43,
	offsetYL = -43,
	hasweapon =  true,	
	damage = 5,
	HP = 100,
	weapon = "w001"
}

player01 = {
	type = 1,
	src = "assets/images/characters/player2.png",
	run = {8, 48, 60, 0},
	idle = {8, 48, 60, 1},	
	dead = {9, 72, 60, 2},
	attack = {},	
	offsetXR = -24,
	offsetXL = -24,
	offsetYR = -43,
	offsetYL = -43,
	hasweapon =  true,	
	damage = 5,
	HP = 100,
	weapon = "w001"
}

w001 = {
	tipe = 1,
	src = "assets/rifle.png",
	shoot = {5, 36, 22, 0},
	gun = "g001"
}

g001 = {
	animatedflag = false,
	src = "assets/bullet2.png",
	value = 1
}