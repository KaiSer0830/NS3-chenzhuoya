clear all;              %%��������ռ�����б���
all_result1 = [];
all_nodeNums = [5; 10; 15; 20; 25; 30];
for i=1:6
    file11 = ['D:\professional_install\matlab\bin\project_files\router_flooding\flooding_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid11 = fopen(file11);
    C11 = textscan(fid11, '%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid11);
    dataRx1 = [C11{5}];                         %%ȡ����5��Ԫ��
    file12 = ['D:\professional_install\matlab\bin\project_files\router_flooding\flooding_RES_TX_', num2str(all_nodeNums(i,:)), '.log'];
    fid12 = fopen(file12);
    C12 = textscan(fid12, '%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid12);
    dataTx1 = [C12{2}];                         %%ȡ����5��Ԫ��
    result = length(dataRx1) / length(dataTx1);
    all_result1 = [all_result1; result];
end
plot(all_nodeNums(:,1), all_result1(:,1), '-->g');
ylabel('Transmission success rate(%)');
xlabel('Density of Nodes');

hold on
all_result2 = [];
for i=1:6
    file21 = ['D:\professional_install\matlab\bin\project_files\router_random\random_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid21 = fopen(file21);
    C21 = textscan(fid21, '%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid21);
    dataRx2 = [C21{5}];                         %%ȡ����5��Ԫ��
    file22 = ['D:\professional_install\matlab\bin\project_files\router_random\random_RES_TX_', num2str(all_nodeNums(i,:)), '.log'];
    fid22 = fopen(file22);
    C22 = textscan(fid22, '%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid22);
    dataTx2 = [C22{2}];                         %%ȡ����5��Ԫ��
    result = length(dataRx2) / length(dataTx2);
    all_result2 = [all_result2; result];
end
plot(all_nodeNums(:,1), all_result2(:,1), '--db');

hold on
all_result3 = [];
for i=1:6
    file31 = ['D:\professional_install\matlab\bin\project_files\router_opport\opport_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid31 = fopen(file31);
    C31 = textscan(fid31, '%f%f%f%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid31);
    dataRx3 = [C31{5}];                         %%ȡ����5��Ԫ��
    file32 = ['D:\professional_install\matlab\bin\project_files\router_opport\opport_RES_TX_', num2str(all_nodeNums(i,:)), '.log'];
    fid32 = fopen(file32);
    C32 = textscan(fid32, '%f%f');          %%����fidΪfopen����ص��ļ���ʶ����formatʵ���Ͼ���һ���ַ�����������ʾ��ȡ���ݼ�����ת���Ĺ���
    fclose(fid32);
    dataTx3 = [C32{2}];                         %%ȡ����5��Ԫ��
    result = length(dataRx3) / length(dataTx3);
    all_result3 = [all_result3; result];
end
plot(all_nodeNums(:,1), all_result3(:,1), '--pr');
legend('Flooding', 'Random', 'Opport');




