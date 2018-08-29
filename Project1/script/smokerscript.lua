function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end

function Smoker_init ()
	_E = { x=0, y=64, w=22, h=22, tox=-16, toy=16, tw=32, th=32, u=32, v=0, uvw=32, uvh=32 }
	_D = { "kill", "0", "spd", "1", "frame", "0", "bul_spd", "3", "shoot_cooldown", "0", "shoot_cooldown_max", "90",
		--   0            1            2              3                  4                          5
		"frame_max", "59", "time", "0", "Y", "0", "X", "0", "attacking", "0", "attack_time_max", "180",
		--  6                 7          8         9             10                11
		"attack_timer", "0", "FRAME", "0", "health", "150"
		--   12                13			 14 
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

	--mepos = getpos(this)

	setpos(this, X + 48 * math.sin(getdatai(this, 7) * math.pi / 180), Y + 4 * math.sin(getdatai(this, 7) * 5 * math.pi / 180))

	sinet = getdatai(this, 7)
	setdatai(this, 7, sinet + 1)

	frame = getdatai(this, 2)
	setdatai(this, 2, frame - 1)
	if (frame < 0) then
		setdatai(this, 2, getdatai(this, 6))
	end
end

function Smoker_hurt()

end

function Smoker_attack()
	if (getdatai(this, 2) % 10 == 0) then
		UV = getuv(this)
		if (UV.u == 32) then
			setuv(this, 64, UV.v)
		else
			setuv(this, 32, UV.v)
		end
	end
end

function Smoker_anim()
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
		UV = getuv(this)
		setuv(this, 96 + (32 * getdatai(this, 13)), UV.v)
	elseif (getdatai(this, 2) % 10 == 0) then
		setdatai(this, 13, 0)
		UV = getuv(this)
		if (UV.u == 32) then
			setuv(this, 64, UV.v)
		else
			setuv(this, 32, UV.v)
		end
	else
		setdatai(this, 13, 0)
	end
end