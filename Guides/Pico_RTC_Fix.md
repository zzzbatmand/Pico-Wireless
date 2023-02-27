# (DEPRICATED) Fixing the RTC
> This has been depricated.  
> See [this](https://github.com/micropython/micropython/issues/10578#issuecomment-1403703934) for why.

<br>

Due to some old   
There is an issue, with the way Real Time Clocks are used in micropython.  
This makes the RTC time and Standard time, incompatable.  
This requires the user to change the format, every time they need to convert between the two.

This guide, show what needs to be fixed, to solve this issue.  
There is an issue report, located [here](https://github.com/micropython/micropython/issues/10578) to see if it has been fixet.

<br>

## Manual fix
You can fix the issue yourselfl by following this guide:  

First open the file at `micropython/ports/rp2/machine_rtc.c`  
In there you need to change the folloing:
``` c
// Find the following struct:
mp_obj_t tuple[8] = {
    mp_obj_new_int(t.year),
    mp_obj_new_int(t.month),
    mp_obj_new_int(t.day),
    mp_obj_new_int(t.dotw),
    mp_obj_new_int(t.hour),
    mp_obj_new_int(t.min),
    mp_obj_new_int(t.sec),
    mp_obj_new_int(0)
};
// And replace that with the following:
mp_obj_t tuple[8] = {
    mp_obj_new_int(t.year),
    mp_obj_new_int(t.month),
    mp_obj_new_int(t.day),
    mp_obj_new_int(t.hour),
    mp_obj_new_int(t.min),
    mp_obj_new_int(t.sec),
    mp_obj_new_int(t.dotw),
    mp_obj_new_int(timeutils_year_day(t.year, t.month, t.day))
};
// NOTE: Just move the "t.dotw" down to second-to-last, and add the "timeutils_year_day" function.
```

Then change this as well, in the same file:  
``` c
// Right below the previous struct, you should see the following:
datetime_t t = {
    .year = mp_obj_get_int(items[0]),
    .month = mp_obj_get_int(items[1]),
    .day = mp_obj_get_int(items[2]),
    .hour = mp_obj_get_int(items[4]),
    .min = mp_obj_get_int(items[5]),
    .sec = mp_obj_get_int(items[6]),
};
// Just change it to this:
datetime_t t = {
    .year = mp_obj_get_int(items[0]),
    .month = mp_obj_get_int(items[1]),
    .day = mp_obj_get_int(items[2]),
    .hour = mp_obj_get_int(items[3]),
    .min = mp_obj_get_int(items[4]),
    .sec = mp_obj_get_int(items[5]),
};
// NOTE: Just fix the numbers, so they go 0-5 in correct order.
```