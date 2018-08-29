function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end

function NPC_init ()
	_E = { x=0, y=0, w=32, h=32, tox=-16, toy=16, tw=32, th=32, u=32, v=0, uvw=32, uvh=32 }
	_D = { "kill", "0", "spd", "1.85" }
	_T = 2
end

function NPC ()
	ppos = getpos(player)
	epos = getpos(this)
	
	mag = dist(epos.x, epos.y, ppos.x, ppos.y)
	velx = getdatai(this, 1) * ( (ppos.x - epos.x) / mag )
	vely = getdatai(this, 1) * ( (ppos.y - epos.y) / mag )
	setpos(this, epos.x + velx, epos.y + vely)
	
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
end