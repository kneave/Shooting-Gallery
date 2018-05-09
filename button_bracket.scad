$fn = 60;

difference()
{
    cube([70,22,22]);
    translate([2, 1, 1]) cube([22,22,22]);
    translate([27, 1, 1]) cube([41,22,22]);
    
    translate([-1,21,-1]) rotate([45,0,0]) cube([72,28,32]);
   
    //  Start/Reset button
    translate([13,10,-0.1]) cylinder(h=22, r=8);
    
    //  set
    translate([38,10,-0.1]) cylinder(h=22, r=3.5);
    //  up
    translate([50,10,-0.1]) cylinder(h=22, r=3.5);
    //  down
    translate([62,10,-0.1]) cylinder(h=22, r=3.5);
}

 