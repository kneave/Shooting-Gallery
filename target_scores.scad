//  Set high feature count
$fn=45;

// small
draw_target(33, 25);


//  medium
translate([50,0,0]) 
union()
{
    draw_target(57, "");
    draw_target(33, 10);
}

//  large
translate([125,0,0]) 
union()
{
    draw_target(78, "");
    draw_target(57, "");
    draw_target(33, 5);
}

// medium
translate([50,-100,0]) 
union()
{
    draw_target(57, "");
    draw_target(33, 10);
}

//  large
translate([125,-100,0]) 
union()
{
    draw_target(78, "");
    draw_target(57, "");
    draw_target(33, 5);
}

module draw_target(diameter, score)
{
    radius = diameter / 2;
    
    difference()
    {
        cylinder(h=3.5, r=radius);
        translate([0,0,2]) cylinder(h=3, r=radius - 1);
    }
    
    translate([0,-7.5,2]) 
        linear_extrude(1.5) 
            text(str(score), halign = "center", size = 15);
}