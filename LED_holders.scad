$fn=30;

difference()
{
    union()
    {
        led_holder();
    }
}
module led_holder()
{
    difference()
    {
        union()
        {            
            cylinder(h=1, r=7);
            rotate([45,0,0]) translate([0,0,-5]) cylinder(h=14, r=5);
        }
        rotate([45,0,0]) translate([0,0,-3]) cylinder(h=19.5, r=3);
        translate([-10,-10,-10]) cube([20,20,10]);
    }
}