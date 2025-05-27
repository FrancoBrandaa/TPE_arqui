GLOBAL get_secs, get_mins, get_hours, get_day, get_month, get_year

get_secs:
    mov al, 0 
	out 70h, al 
	in al, 71h 
	ret

get_mins:
    mov al, 2 
	out 70h, al 
	in al, 71h 
	ret
    
get_hours:
    mov al, 4
	out 70h, al 
	in al, 71h 
	ret

get_day:
    mov al, 7 
	out 70h, al 
	in al, 71h 
	ret

get_month:
    mov al, 8 
	out 70h, al 
	in al, 71h 
	ret

get_year:
    mov al, 9
	out 70h, al 
	in al, 71h 
	ret