function Text_init ()
	_E = { x=-112, y=112, w=0, h=0, tox=-2, toy=3, tw=4, th=8, u=0, v=0, uvw=4, uvh=8 }
	_D = { "kill", "0", "spacing", "1" }
	--		 0              1
	_T = 6
end

function Text ()
	--[[
	if (getdatai(this, 2) == 1) then
		Text_set("Hello!")
		setdatai(this, 2, 0)
	end
	]]
end

function Text_set(str)
	
end