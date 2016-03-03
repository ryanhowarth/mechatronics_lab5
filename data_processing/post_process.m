clc; close all; 

right_side = 1:1:79;
left_side = 1:1:79;
z_axis = 1:1:79;

[data] = importdata('tape_measurments.csv');
Tape_Left = data.data(2:80, 3);
Tape_Right = data.data(2:80, 4);



[data] = importdata('raw_data.txt');

for i=1:1:78
    index = (i*4) -2;
    left_side(i) = VarName6(index);
    index = index + 1;
    right_side(i) = VarName6(index);
    index = i*4;
    z_axis(i) = from(index);
end

z_axis = z_axis./100;
hold on;
plot(z_axis, left_side);
plot(z_axis, -right_side);
plot(z_axis, Tape_Left);
plot(z_axis, -Tape_Right);


plot(z_axis, z_axis.*0, '-');

title('Mapping of Wyman 157');
xlabel('Position On Track (m)');
ylabel('Distance away from Track (cm)');
legend('Left Sensor','Right Sensor', 'Tape Measure Left', 'Tape Measure Right', 'Track', 'Location', 'BestOutside');

size(left_side)
size(Tape_Left)

disp('left average error');
abs(mean(left_side(1:77)-Tape_Left(1:77)'))
disp('right average error');
abs(mean(right_side(1:77)-Tape_right(1:77)'))


    


