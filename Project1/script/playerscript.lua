function Player_init ()
	_E = { x=0, y=0, w=32, h=32, tox=-16, toy=16, tw=32, th=32, u=0, v=32, uvw=32, uvh=32 }
	_D = { "kill", "0", "spd", "4", "proj_spd", "8", "shoot_cooldown", "0", "shoot_cooldown_max", "10" }
	_T = 3
end

function Player ()
	Player_anim()
	spd = getdatai(this, 1)

	input = get_input()
	if (input.W > 0) then
		setpos(this, getpos(this).x, getpos(this).y + spd)
	end
	if (input.A > 0) then
		setpos(this, getpos(this).x - spd, getpos(this).y)
	end
	if (input.S > 0) then
		setpos(this, getpos(this).x, getpos(this).y - spd)
	end
	if (input.D > 0) then
		setpos(this, getpos(this).x + spd, getpos(this).y)
	end
	if (input.UP > 0) then
		shoot(0)
	end
	if (input.LEFT > 0) then
		shoot(1)
	end
	if (input.DOWN > 0) then
		shoot(2)
	end
	if (input.RIGHT > 0) then
		shoot(3)
	end

	cooldown = getdatai(this, 3)
	setdatai(this, 3, cooldown - 1)
	if (cooldown < 0) then
		setdatai(this, 3, 0)
	end
end

function shoot(dir)
	if (getdatai(this, 3) <= 0) then
		Pr = gen_ent(master, "Projectile")
		if (Pr ~= 0) then
			ppos = getpos(this)
			if (dir == 0) then
				setdatai(Pr, 3, 0)
				setdatai(Pr, 4, getdatai(this, 2))
				setpos(Pr, ppos.x, ppos.y + 16)
			end
			if (dir == 1) then
				setdatai(Pr, 3, getdatai(this, 2) * -1)
				setdatai(Pr, 4, 0)
				setpos(Pr, ppos.x - 16, ppos.y)
			end
			if (dir == 2) then
				setdatai(Pr, 3, 0)
				setdatai(Pr, 4, getdatai(this, 2) * -1)
				setpos(Pr, ppos.x, ppos.y - 16)
			end
			if (dir == 3) then
				setdatai(Pr, 3, getdatai(this, 2))
				setdatai(Pr, 4, 0)
				setpos(Pr, ppos.x + 16, ppos.y)
			end
			setdatai(this, 3, getdatai(this, 4))
		end
	end
end

function Player_anim()
	if (getdatai(this, 3) >= getdatai(this, 4) - 3) then
		setuv(this, 0, 0)
	else
		setuv(this, 0, 32)
	end
end