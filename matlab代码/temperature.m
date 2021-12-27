clear all;              %%��������ռ�����б���
all_data = [];
for i=1:13
    file = ['nanonodeposition',num2str(i),'.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid);
    data = [C{1},C{6}];                         %%ȡ����1�����6��Ԫ��
    data = data(100:100:end, :);                %%����ǰ������У���������С���һ��Ԫ�ش���ȡ����ʼ�ţ�Ϊ��200�У��ڶ���Ԫ�ش���ȡ�ļ����200���ڶ�����ȡ��400�У�������Ԫ�ش���ȡ��ĩβ��
    all_data = [all_data; data];
 
    plot(data(:,1), data(:,2), '--');
    ylabel('Temperatrue increase(��)');
    xlabel('Time');
end

all_data = all_data(:,2:2:end);             %%ֻȡ��
idx = find(all_data(:,1)~=0);               %%ȡ������Ԫ��
data2 = all_data(idx,:);

x = 1e-10:1e-9:1e-6;
y = zeros(1,length(x));
for i=1:length(y)
    y(i) = size(find(data2 < x(i)), 1);
end
y = y./341;
plot(x, y, '-r', 'LineWidth', 2);
xlabel('Temperatrue increase ��T(K)');
ylabel('Cumulative distribution function(CDF)');
legend('simulation of CDF')
%hold on�����Ǳ���ԭͼ�����ܴ˺���Ƶ��µ�����,���ӻ�ͼ; 
hold on


