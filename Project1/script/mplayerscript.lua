function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end
function sign(x)
	return x / math.abs(x)
end

function MPlayer_init ()
	_E = { x=0, y=0, w=13, h=13, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16 }
	_D = { "kill", "0", "spd", "1", "shoot_cooldown", "0", 
		--   0            1            2
		"shoot_cooldown_max", "10", "velx", "0", "vely", "0", 
		--   3                         4            5
		"accl", "0.05", "friction", "0.02", "maxvel", "1.75",
		-- 6                7                 8
		"dash_cooldown", "0", "dash_cooldown_max", "60", "dash_vel", "7.5",
		--    9                        10                     11
		"dash_frame", "0", "dash_frame_max", "3", "dash_ef_frame", "0",
		--    12                  13					14
		"dash_ef_frame_max", "5", "dash_unix", "0", "dash_uniy", "0",
		--   15                       16               17
		"roll_anim_frame", "0", "roll_anim_frame_max", "5", "roll_anim_state", "0"
		--   18                        19                             20
	}
	_T = 1
end

function MPlayer ()
	MPlayer_anim()
	spd = getdatai(this, 1)
	input = get_input()
	if (input.W > 0) then
		setdatai(this, 5, getdatai(this, 5) + getdatai(this, 6))
		--setpos(this, getpos(this).x, getpos(this).y + spd)
		--setuv(this, 48, 0)
	end
	if (input.S > 0) then
		setdatai(this, 5, getdatai(this, 5) - getdatai(this, 6))
		--setpos(this, getpos(this).x, getpos(this).y - spd)
		--setuv(this, 0, 0)
	end
	if (input.A > 0) then
		setdatai(this, 4, getdatai(this, 4) - getdatai(this, 6))
		--setpos(this, getpos(this).x - spd, getpos(this).y)
		--setuv(this, 16, 0)
	end
	if (input.D > 0) then
		setdatai(this, 4, getdatai(this, 4) + getdatai(this, 6))
		--setpos(this, getpos(this).x + spd, getpos(this).y)
		--setuv(this, 32, 0)
	end

	xpol = sign(getdatai(this, 4))
	ypol = sign(getdatai(this, 5))
	if (math.abs(getdatai(this, 4)) > 0) then
		setdatai(this, 4, getdatai(this, 4) + (xpol * -1 * getdatai(this, 7)))
		if (sign(getdatai(this, 4)) ~= xpol) then 
			setdatai(this, 4, 0) 
		end
	end
	if (math.abs(getdatai(this, 5)) > 0) then
		setdatai(this, 5, getdatai(this, 5) + (ypol * -1 * getdatai(this, 7)))
		if (sign(getdatai(this, 5)) ~= ypol) then 
			setdatai(this, 5, 0) 
		end
	end
	if (math.abs(getdatai(this, 4)) > getdatai(this, 8)) then
		setdatai(this, 4, sign(getdatai(this, 4)) * getdatai(this, 8))
	end
		if (math.abs(getdatai(this, 5)) > getdatai(this, 8)) then
		setdatai(this, 5, sign(getdatai(this, 5)) * getdatai(this, 8))
	end

	ppos = getpos(this)
	mag = dist(ppos.x, ppos.y, CURSOR_X, CURSOR_Y)
	unix = (CURSOR_X - ppos.x) / mag
	uniy = (CURSOR_Y - ppos.y) / mag
	if (getdatai(this, 12) <= 0) then
		setdatai(this, 16, unix)
		setdatai(this, 17, uniy)
	end
	if (MOUSE_RIGHT == 1) then
		if (getdatai(this, 9) <= 0) then
			setdatai(this, 9, getdatai(this, 10))
			setdatai(this, 12, getdatai(this, 13))
			setdatai(this, 14, getdatai(this, 15))
		end
	end

	if (MOUSE_LEFT == 1) then
		MPlayer_shoot(unix, uniy)
	end
	if (getdatai(this, 12) > 0) then
		MPlayer_dash(getdatai(this, 16), getdatai(this, 17))
		--MPlayer_dash(1/math.sqrt(2), 1/math.sqrt(2))
	end

	if (getdatai(this, 14) > 0) then
		MPlayer_eff_puff(ppos.x, ppos.y)
	end

	setpos(this, getpos(this).x + getdatai(this, 4), getpos(this).y + getdatai(this, 5))

	s_cooldown = getdatai(this, 2)
	setdatai(this, 2, s_cooldown - 1)
	if (s_cooldown < 0) then
		setdatai(this, 2, 0)
	end
	d_cooldown = getdatai(this, 9)
	setdatai(this, 9, d_cooldown - 1)
	if (d_cooldown < 0) then
		setdatai(this, 9, 0)
	end
	d_frame = getdatai(this, 12)
	setdatai(this, 12, d_frame - 1)
	if (d_frame < 0) then
		setdatai(this, 12, 0)
	end
	d_eff_frame = getdatai(this, 14)
	setdatai(this, 14, d_eff_frame - 1)
	if (d_eff_frame < 0) then
		setdatai(this, 14, 0)
	end
end

function MPlayer_shoot(unit_x, unit_y)
	if (getdatai(this, 2) <= 0) then
		Pr = gen_ent(master, "BasicShot")
		if (Pr ~= 0) then
			ppos = getpos(this)
			setdatai(Pr, 3, unit_x * 2.5)
			setdatai(Pr, 4, unit_y * 2.5)
			setpos(Pr, ppos.x, ppos.y)

			setdatai(this, 2, getdatai(this, 3))
		end
	end
end

function MPlayer_dash(unit_x, unit_y)
	setdatai(this, 4, unit_x * getdatai(this, 11))
	setdatai(this, 5, unit_y * getdatai(this, 11))
end

function MPlayer_eff_puff(x, y)
	Ef = gen_ent(master, "BasicShotPuff")
	setpos(Ef, x, y)
end

function MPlayer_anim()
	roll_anim_frame = getdatai(this, 18)

	setdatai(this, 18, roll_anim_frame - math.sqrt(getdatai(this, 4)*getdatai(this, 4)+getdatai(this, 5)*getdatai(this, 5)))
	if (roll_anim_frame < 0) then
		if (getdatai(this, 20) == 0) then setdatai(this, 20, 1) else setdatai(this, 20, 0) end
		setdatai(this, 18, getdatai(this, 19))
	end

	setuv(this, 16 * getdatai(this, 20), 0)
end