function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end
function get_angle(x1, y1, x2, y2)
	if (x1 <= x2) then
		theta = math.atan((y2 - y1)/(x2 - x1))
		if (theta < 0) then
			return 2 * math.pi + theta
		else
			return theta
		end
	else
		return math.pi - math.atan((y2 - y1)/math.abs(x2 - x1))
	end
end

function Fume_init ()
	_E = { x=0, y=0, w=16, h=16, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16 }
	_D = { "kill", "0", "spd", "1", "frame", "0", "bul_spd", "3", "shoot_cooldown", "0", "shoot_cooldown_max", "90",
		--   0            1            2              3                  4                          5
		"frame_max", "29"
		--  6
	}

	_T = 2
end

function Fume ()
	Fume_anim()

	mepos = getpos(this)
	ppos = getpos(player)
	mag = dist(mepos.x, mepos.y, ppos.x, ppos.y)
	unix = (ppos.x - mepos.x) / mag
	uniy = (ppos.y - mepos.y) / mag
	setpos(this, mepos.x + (unix * 1), mepos.y + (uniy * 1))

	frame = getdatai(this, 2)
	setdatai(this, 2, frame - 1)
	if (frame < 0) then
		setdatai(this, 2, getdatai(this, 6))
	end
end

function Fume_anim()
	mepos = getpos(this)
	ppos = getpos(player)
	angle = get_angle(mepos.x, mepos.y, ppos.x, ppos.y)
	V = 0
	if (angle < (math.pi / 4) or angle >= (2 * math.pi * 7) / 8) then
		V = 32
	elseif (angle >= (math.pi / 4) and angle < (2 * math.pi * 3) / 8) then
		V = 48
	elseif (angle >= (2 * math.pi * 3) / 8 and angle < (2 * math.pi * 5) / 8) then
		V = 16
	else
		V = 0
	end

	if (getdatai(this, 2) % 15 == 0) then
		Ef = gen_ent(master, "BasicShotPuff")
		setpos(Ef, getpos(this).x, getpos(this).y)
		setuv(Ef, 0, 8)
	end

	if (getdatai(this, 2) % 5 == 0) then
		UV = getuv(this)
		if (UV.u == 16) then
			setuv(this, 0, V)
		else
			setuv(this, 16, V)
		end
	end
end