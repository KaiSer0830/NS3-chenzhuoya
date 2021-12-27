clear all;              %%��������ռ�����б���
all_result1 = [];
all_nodeNums = [5; 10; 15; 20; 25; 30];
for i=1:6
    file = ['D:\professional_install\matlab\bin\project_files\router_flooding\flooding_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid);
    data = [C{5}];                         %%ȡ����5��Ԫ��
    s = sum(data);
    result = s / length(data);
    all_result1 = [all_result1; result];
end
plot(all_nodeNums(:,1), all_result1(:,1), '-->g');
ylabel('Delay(ps)');
xlabel('Density of Nodes');

hold on
all_result2 = [];
for i=1:6
    file = ['D:\professional_install\matlab\bin\project_files\router_random\random_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid);
    data = [C{5}];                         %%ȡ����5��Ԫ��
    s = sum(data);
    result = s / length(data);
    all_result2 = [all_result2; result];
end
plot(all_nodeNums(:,1), all_result2(:,1), '--db');

hold on
all_result3 = [];
for i=1:6
    file = ['D:\professional_install\matlab\bin\project_files\router_opport\opport_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid);
    data = [C{5}];                         %%ȡ����5��Ԫ��
    s = sum(data);
    result = s / length(data);
    all_result3 = [all_result3; result];
end
plot(all_nodeNums(:,1), all_result3(:,1), '--pr');
legend('Flooding', 'Random', 'Opport');




