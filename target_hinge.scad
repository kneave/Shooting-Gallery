$fn = 40;

//translate([-16.5,0,-2]) cube([42,22,1]);


//  Length from pivot to target edge
//  Old hinge 30, new hinge 26 (double check)
base_length = 24;
target_radius = 41;
gap = 1;


draw_arm(target_radius, base_length, 1);

//  Arm length is 55mm + target diameter
//  Large: 82, Medium: 60, Small: 35
        
module draw_arm(target_radius, base_length, gap)
{   
    target_holder = 15;
    length = target_radius + base_length + gap + target_holder;
    
//  Axle hole
    difference()
    {
        union()
        {            
            //  Render the cylinder which holds the axle
            cylinder(h=12.8, r1=2.5, r2=2.5);
            
            //  Render the full length of the arm
            translate([1.2,0,0]) cube([1.3,length,12.8]);
            
            //  Magnet holder
            color("green") translate([-0.7,0,0]) cube([3,24,12.8]);
            
            //  Render the two support sections
            translate([-1.8,0,0]) cube([3,length,1]);
            translate([-1.8,0,11.8]) cube([3,length,1]);
            
            //  LED holder
            translate([2.5,length - 15,6.4]) rotate([0,-90,0]) cylinder(h=7.2, r=5);
            
            //  target holders
            translate([-4.8,length - 2 ,0]) cube([7,2,12.8]);
            translate([-4.8,length - 2 * target_holder,0]) cube([7,2,12.8]);
        }
                
        //  Remove the hole for the axle
        translate([0,0,-0.1]) cylinder(h=13, r1=1.2, r2=1.2);
        
        translate([4.1,length - 15,6.4]) rotate([0,-90,0]) translate([0,0,0.5]) cylinder(h=8.4, r=3);
    }
}

//  Backstop
module backstop()
{
    translate([12,2,1]) cube([2,16,9]);
    
    color("orange") 
            rotate([90,0,90]) translate([11,10,12])  
                cylinder(h=2, r1=8, r2=8);    
    
    //  rear chamfer
    difference()
    {
        translate([12,10,0]) cube([8,2,11.4]);
        color("purple") 
            translate([20,9,1]) rotate([0,-30,0]) 
                cube([6,5,12]);                       
    }
}

//  Methods
module side(servo_mount)
{
    if(servo_mount){
            color("blue") 
                translate([2,5.3,0]) cube([8,1,10]);
        
            color("green") 
                translate([10,3.3,0]) cube([1,3,10]);
    }

    //  Top edge
    difference()
    {
        translate([0,2,0]) cube([14,2,10]);
        
        if(servo_mount)
        {
            color("blue") 
                translate([0,1.9,0]) cube([5,3,15]);
        }
        else
        {
            //  Front chamfer
            color("blue") 
                translate([-5,1.9,2.3]) rotate([0,30,0]) 
                    cube([5,3,15]);
        }
        //  Axle hole
        color("green") 
            rotate([90,0,0]) translate([7,6,-4.1])  
                cylinder(h=5, r1=1.1, r2=1.1);
    }    
    
    //  Bottom edge
    if(servo_mount)
    {
        difference()
        {
            translate([12,2,0]) cube([14,2,10]);
            
            //  Screw hole
            color("green") 
                rotate([90,0,0]) translate([20,7.5,-5.1])  
                    cylinder(h=5, r=0.75);
        }
    }
    else
    {
        //  rear chamfer
        difference()
        {
            translate([12,2,0]) cube([8,2,10]);
                color("purple") 
                    translate([20,1.9,1]) rotate([0,-30,0]) 
                        cube([6,5,12]);        
        }
    }
}

module draw_hinge(){
    difference(){
        union()
        {
            backstop();
            side();
            translate([-2,0,0]) cube([28,22,1]);
            translate([0,16,0]) side(true);
            translate([-5,11,0]) cylinder(h=2, r1=11.5, r2=11.5);  
        }
        translate([-5,11,0.7]) cylinder(h=26, r1=10.5, r2=10.5);  
    }
}

module led_holder()
{
    difference()
    {
        
    }
}