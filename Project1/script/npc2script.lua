function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end

function NPC2_init ()
	_E = { x=0, y=0, w=32, h=32, tox=-16, toy=16, tw=32, th=32, u=32, v=32, uvw=32, uvh=32 }
	_D = { "kill", "0", "spd", "1", "frame", "0", "bul_spd", "3", "shoot_cooldown", "0", "shoot_cooldown_max", "90" }
	--		 0            1            2              3                  4                          5
	_T = 2
end

function NPC2 ()
	NPC2_anim()

	ppos = getpos(player)
	epos = getpos(this)
	
	mag = dist(epos.x, epos.y, ppos.x, ppos.y)

	unix = (ppos.x - epos.x) / mag
	uniy = (ppos.y - epos.y) / mag
	velx = getdatai(this, 1) * unix
	vely = getdatai(this, 1) * uniy

	setpos(this, epos.x + velx, epos.y + vely)
	NPC2_shoot(unix, uniy)
	
	n = 0
	while (n < get_entcount(master)) do
		Pr = get_ent(master, n)
		if (get_ent_type(Pr) == 1) then
			Prpos = getpos(Pr)
			if ((dist(Prpos.x, Prpos.y, epos.x, epos.y) < 28) and (getdatai(Pr, 5) ~= 1)) then
				setdatai(Pr, 5, 1)
				Ef = gen_ent(master, "Effect")
				setpos(Ef, epos.x, epos.y)
				setdatai(this, 0, 2)
			end
		end
		n = n + 1
	end

	cooldown = getdatai(this, 4)
	setdatai(this, 4, cooldown - 1)
	if (cooldown < 0) then
		setdatai(this, 4, 0)
	end
end

function NPC2_shoot(unit_x, unit_y)
	if (getdatai(this, 4) <= 0) then
		Pr = gen_ent(master, "Projectile2")
		if (Pr ~= 0) then
			npos = getpos(this)
			setdatai(Pr, 3, unit_x * getdatai(this, 3))
			setdatai(Pr, 4, unit_y * getdatai(this, 3))
			setpos(Pr, npos.x + unit_x * 28, npos.y + unit_y * 28)

			setdatai(this, 4, getdatai(this, 5))
		end
	end
end

function NPC2_anim()
	if (getdatai(this, 4) >= getdatai(this, 5) - 3) then
		setuv(this, 64, 32)
	else
		setuv(this, 32, 32)
	end
end