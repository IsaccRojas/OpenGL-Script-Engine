function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end

function Smoker_init ()
	_E = { x=0, y=64, w=22, h=22, tox=-16, toy=16, tw=32, th=32, u=32, v=0, uvw=32, uvh=32 }
	_D = { "kill", "0", "spd", "1", "frame", "0", "bul_spd", "3", "shoot_cooldown", "0", "shoot_cooldown_max", "90",
		--   0            1            2              3                  4                          5
		"frame_max", "59", "time", "0", "Y", "0", "X", "0", "attacking", "0", "attack_time_max", "180",
		--  6                 7          8         9             10                11
		"attack_timer", "0", "FRAME", "0", "health", "150", "hurt_cooldown", "0", "hurt_cooldown_max", "10",
		--   12                13			 14 				15						16
		"flash_dir", "2", "flash_state", "1"
		--  17				   18
	}

	_T = 2
end

function Smoker ()
	Smoker_anim()

	if (getdatai(this, 7) % 480 == 0 and getdatai(this, 7) ~= 0) then
		setdatai(this, 10, 1)
	end

	Y = 62
	X = 0
	if (getdatai(this, 10) == 1) then
		if (getdatai(this, 7) % 30 == 0 and getdatai(this, 12) > 20 and getdatai(this, 12) < 160) then
			Fu = gen_ent(master, "Fume")
			setpos(Fu, X + 48 * math.sin(getdatai(this, 7) * math.pi / 180), Y + 4 * math.sin(getdatai(this, 7) * 5 * math.pi / 180))		
		end

		a_frame = getdatai(this, 12)
		setdatai(this, 12, a_frame + 1)
		if (a_frame >= getdatai(this, 11)) then
			setdatai(this, 10, 0)
			setdatai(this, 12, 0)
		end
	end
	Bullets, size = get_ent_all_tn(master, 4, "BasicShot")
	if (size > 0) then
		pos = getpos(this)
		i = 0
		hits = 0
		while (i < size) do
			bullet_pos = getpos(Bullets[i])
			if (dist(pos.x, pos.y, bullet_pos.x, bullet_pos.y) <= 14) then
				setdatai(Bullets[i], 5, 1)
				hits = hits + getdatai(Bullets[i], 6)
			end
			i = i + 1
		end
		if (hits > 0 and getdatai(this, 10) > 0) then
			setdatai(this, 14, getdatai(this, 14) - hits)
			setdatai(this, 15, getdatai(this, 16))
			--...
		end
	end
	if (getdatai(this, 14) <= 0) then
		setdatai(this, 0, 2)
		cur_pos = getpos(this)
		Ef1 = gen_ent(master, "SmokerPuff")
		setpos(Ef1, cur_pos.x - 4, cur_pos.y + 8)
		setdatai(Ef1, 3, -1)
		setdatai(Ef1, 4, 1)
		Ef2 = gen_ent(master, "SmokerPuff")
		setpos(Ef2, cur_pos.x - 4, cur_pos.y - 8)
		setdatai(Ef2, 3, -1)
		setdatai(Ef2, 4, -1)
		setuv(Ef2, 72, 24)
		Ef3 = gen_ent(master, "SmokerPuff")
		setpos(Ef3, cur_pos.x + 4, cur_pos.y - 8)
		setdatai(Ef3, 3, 1)
		setdatai(Ef3, 4, -1)
		setuv(Ef3, 72, 48)
		Ef4 = gen_ent(master, "SmokerPuff")
		setpos(Ef4, cur_pos.x + 4, cur_pos.y + 8)
		setdatai(Ef4, 3, 1)
		setdatai(Ef4, 4, 1)
		setuv(Ef4, 72, 72)
	end
	if (getdatai(this, 15) > 0) then
		setdatai(this, 15, getdatai(this, 15) - 1)
	end

	setpos(this, X + 48 * math.sin(getdatai(this, 7) * math.pi / 180), Y + 4 * math.sin(getdatai(this, 7) * 5 * math.pi / 180))

	sinet = getdatai(this, 7)
	setdatai(this, 7, sinet + 1)

	frame = getdatai(this, 2)
	setdatai(this, 2, frame - 1)
	if (frame < 0) then
		setdatai(this, 2, getdatai(this, 6))
	end
end

function Smoker_anim()
	UV = getuv(this)
	if (getdatai(this, 15) > 0) then
		if ((getdatai(this, 7) % getdatai(this, 17)) == 0) then
			state = getdatai(this, 18)
			if (state == 0) then
				setdatai(this, 18, 1)
			else
				setdatai(this, 18, 0)
			end
		end
		setuv(this, UV.u, 32 * getdatai(this, 18))
	else
		setuv(this, UV.u, 0)
	end

	UV = getuv(this)
	if (getdatai(this, 10) == 1) then
		if (getdatai(this, 12) < 16 and getdatai(this, 12) % 3 == 0) then
			setdatai(this, 13, getdatai(this, 13) + 1)
			if (getdatai(this, 13) > 4) then
				setdatai(this, 13, 4)
			end
		elseif (getdatai(this, 12) > (180 - 16) and getdatai(this, 12) % 3 == 0) then
			setdatai(this, 13, getdatai(this, 13) - 1)
			if (getdatai(this, 13) < 0) then
				setdatai(this, 13, 0)
			end
		end
		setuv(this, 96 + (32 * getdatai(this, 13)), UV.v)
	elseif (getdatai(this, 2) % 10 == 0) then
		setdatai(this, 13, 0)
		if (UV.u == 32) then
			setuv(this, 64, UV.v)
		else
			setuv(this, 32, UV.v)
		end
	else
		setdatai(this, 13, 0)
	end
end