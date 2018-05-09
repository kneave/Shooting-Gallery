//  Draw the plaque

//  Draw the gauge with servo cuttouts
difference()
{
    union()
    {
        difference()
        {
            color("blue") 
                translate([-52.8,-10,0]) cube([112,73,1.5]);
            translate([-51.8,-9,00.5]) cube([110,71,2]);
        }
        rotate([0,0,240]) draw_gauge();
    }
    $fn = 15;
    translate([0,0,-0.1]) cylinder(h=5, r=6);
    translate([6.2,0,-0.1]) cylinder(h=5, r=2.5);
}


//  Various modules

module draw_gauge()
{
    //  Draw the edges
    mark(0,0.5,55,0);
    mark(1,0.5,55,0);

    //  Draw the major lines and labels
    for(i = [0 : 0.1 : 1])
    {
        mark(i,1,15,40);
        label(i, i * 100);
    }

    //  Draw the minor lines
    for(i = [0 : 0.01 : 1])
    {
        mark(i,0.2,5,50);
    }

}

module label(percent, value)
{
    //  0% = 180
    //  100% = 60
    converted_angle = 177.5 - (120 * percent);
    
       //text(text = str(text,"_",a[1]), font = font, size = 20, halign = a[1]);
     //}
    
    linear_extrude(height = 1.5) 
    {
    
    //  180 is left extent, 60 is right extent
    rotate([0,0,converted_angle]) 
        translate([0, 55,0]) 
            rotate([0,0,90]) text(text = str(value), size = 4.5);
    }        
}

//  Percent is the relative angle on the dial where left is 0 and right is 1
//  Thickness is how wide
//  Length is length 
//  Margin is how far from the centre point
module mark(percent, thickness, length, margin)
{
    //  0% = 180
    //  100% = 60
    converted_angle = 180 - (120 * percent);
    offset = thickness / 2;
    
    //  180 is left extent, 60 is right extent
    rotate([0,0,converted_angle]) 
        translate([-offset, margin,0]) 
            cube([thickness,length,1.5]);
}


/* 
 * Excerpt from... 
 * 
 * Parametric Encoder Wheel 
 *
 * by Alex Franke (codecreations), March 2012
 * http://www.theFrankes.com
 * 
 * Licenced under Creative Commons Attribution - Non-Commercial - Share Alike 3.0 
*/
module arc(innerradius, radius, height, degrees ) 
{
    $fn=60;
    
    render() 
    {
            if( degrees > 180 )
            {
                difference()
                {
                  difference()
                    {
                        cylinder( r=radius, h=height );
                        cylinder( r=innerradius*1.0001, h=height );
                    }
                    rotate([0,0,-degrees]) arc( innerradius,radius*1.0001, height*1.0001,  360-degrees) ;                    
                }
            }
            else
            {        
                difference() 
                {    
                    // Outer ring
                    rotate_extrude()
                        translate([innerradius, 0, 0])
                            square([radius - innerradius,height]);
                
                    // Cut half off
                    translate([0,-(radius+1),-.5]) 
                        cube ([radius+1,(radius+1)*2,height+1]);
           
                    // Cover the other half as necessary
                    rotate([0,0,180-degrees])
                    translate([0,-(radius+1),-.5]) 
                        cube ([radius+1,(radius+1)*2,height+1]);
                }
            }
        
    }
}