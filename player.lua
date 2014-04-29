--animation arraydata: nro frame, width, height, nrorow
mob001 = {
	src = "assets/enemmy.png",
	start = {5, 70, 40, 2},
	idle = {4, 70, 40, 1},
	run = {4, 70, 40, 0},
	attack  = {4, 70, 40, 3},
	offsetXR = -47,
	offsetXL = -23,
	offsetYR = -20,
	offsetYL = -20,
	hasweapon = false;
	weapon  = "";
}

player01 = {
	src = "assets/player.png",
	run = {8, 48, 60, 0},
	idle = {8, 48, 60, 1},
	attack = {},	
	offsetXR = -24,
	offsetXL = -24,
	offsetYR = -35,
	offsetYL = -35,
	hasweapon =  true,
	weapon = "w001"
}

w001 = {
	tipe = 1,
	src = "assets/rifle.png",
	shoot = {5, 36, 22, 0},
	gun = "g001"
}

g001 = {
	src = "bullet2.png",
	value = 1
}

player = {
    position = {
    x = 32.5, y = 20.0
    },
    filename = "sprite.png",
    HP = 300
}

array2 = {1, 1, 2, 3, 5, 10, 20}