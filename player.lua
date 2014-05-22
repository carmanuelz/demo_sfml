--animation arraydata: nro frame, width, height, nrorow
mob001 = {
	type = 4,
	src = "assets/enemmy.png",
	start = {5, 70, 40, 2},
	idle = {4, 70, 40, 1},
	run = {4, 70, 40, 0},
	attack  = {4, 70, 40, 3},
	offsetXR = -47,
	offsetXL = -23,
	offsetYR = -20,
	offsetYL = -20,
	hasweapon = false,
	damage = 5,
	HP = 40,
	weapon  = "",
}

player01 = {
	type = 1,
	src = "assets/player2.png",
	run = {8, 48, 60, 0},
	idle = {8, 48, 60, 1},	
	dead = {9, 72, 60, 2},
	attack = {},	
	offsetXR = -24,
	offsetXL = -24,
	offsetYR = -35,
	offsetYL = -35,
	hasweapon =  true,	
	damage = 5,
	HP = 20,
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