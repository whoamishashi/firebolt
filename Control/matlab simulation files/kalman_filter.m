a=size(time_in_s);
R=0.001;
x=zeros(1,a(1));
z=gyro;
rx=zeros(1,a(1));
p=[100,zeros(1,a(1)-1)];
K=zeros(1,a(1));
rp=zeros(1,a(1));
m=zeros(1,a(1));
for i=2:1:size(time_in_s)
    m(i)=(z(i)-z(i-1));
    rp(i)=p(i-1);
    K(i)=rp(i)/(rp(i)+R);
    rx(i)=x(i-1);
    x(i)=rx(i)+(1-K(i))/4*(z(i)-rx(i));
    p(i)=(1-K(i))*rp(i);
end
plot(time_in_s,x','g','LineWidth',4)
hold on
plot(time_in_s,z,'r')