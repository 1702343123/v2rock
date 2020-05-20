# v2rock
GUI tool for v2ray  
���Ubuntu���а��v2ray�������. ������ʹ�ö��ĵ�ַ��v2ray����. 

## ����Ҫ��

### ����ϵͳҪ��  
���������������Ubuntu��Xenial(16.04), Bionic(18.04), Focal(20.04) ��64λ�汾, ����Debian���а�ķ������������.  

### ��װv2ray
���������Я��v2ray, ��Ҫ�����а�װv2ray, �ο�[v2ray Github��Ŀ��ַ](https://github.com/v2ray/v2ray-core), [v2ray����](https://www.v2ray.com/).

**���簲װ**  

ִ�й����ṩ��shell�ű�  
```shell
bash <(curl -L -s https://install.direct/go.sh)
```

**���ذ�װ**  

�����绷�����õĵط�, ���簲װ���ܻ�ʧ��, ��Ҫ�����´���:  
1. �� [install.direct](https://install.direct/go.sh)����go.sh�ű�
1. ��[����Release](https://github.com/v2ray/v2ray-core/releases), [Github Dist](https://github.com/v2ray/dist/)����Ԥ�����ѹ����, ��ӦUbuntu 64λ, Ӧ�������ļ���Ϊ v2ray-linux-64.zip ���ļ�.
1. �ڱ���ִ����������װ, �汾����ʹ�õ�Ԥ����ѹ�����İ汾
```
./go.sh --version v1.13 --local /path/to/v2ray.zip
```

### �����������

�Ƽ�ʹ�� SwitchyOmega ���, [SwitchyOmega Github��Ŀ��ַ](https://github.com/FelisCatus/SwitchyOmega). �˲��֧��Firefox��Chrome, ʹ�÷���, ����������ϸ���Э��Ĵ���.  

## ��װ

���ض�Ӧ���а��deb�ļ�������, ��dpkg��װ, ��װ��ҪsudoȨ��
```shell
sudo dpkg -i v2rock_x.x.x-x_xxxxx_xxxxx.deb
```
�����ʾ��δ��װ������, ����������������޸�������ϵ
```shell
sudo apt install -f
```
�����Ҫ�鿴deb��װ��������, ����ʹ��
```shell
dpkg -c v2rock_x.x.x-x_xxxxx_xxxxx.deb
```

## ʹ��

### ����
��װ�ɹ���, ������б��п����ҵ�v2rock, ���߰���`super`��(`win`��)������v2rock,�����v2rock��ͼ��, �������. v2rock��һ�����л��� ~/.config/v2rock/ �³�ʼ�������ļ�. ����Ҫ���`settings`��ť, ��`general`��ǩ���������²���  

* Subscribe URL: ����д�������ṩ�Ķ��ĵ�ַ
* Port: Ĭ��Ϊ1080, ����ĳɱ�Ķ˿���Ҫע�ⲻҪ��������Ӧ�ó�ͻ, ���ⲻ��С��1024, ��ΪС��1024�Ķ�����ϵͳ�����˿�, ��Ҫsudo Ȩ��
* V2Ray Executable Path: v2ray�Ŀ�ִ���ļ���ַ, Ĭ�ϰ�װ��/usr/bin/v2ray/v2ray, ���û�����Ͳ���Ҫ�޸�.

### ����
�������궩�ĵ�ַ��, ���`Import`��ť����, ������벻�ɹ�, ��������������Զ��ĵ�ַ�Ƿ���ֱ�ӷ���, ������ʲ���, ˵����ַ�޷�����, ��Ҫ��ϵ���ĵ�ַ�ṩ�����.  

### ����
��ʱֻ�ṩping����, ����һ���ɹ�����ڵ��, ���б����������ѡ�нڵ�, �ٵ��`Test`��ť, �Ϳ��Բ��Ե�ǰѡ�нڵ��ping, ���·���־���ڿ��Կ����ڵ��ping���.  
�ڲ��Թ�����, ����ڵ��޷�����, ��־���ڳ�ʱ�������, ���Ե��`Stop`��ťֹͣ.

### ����
���б���˫��ѡ�еĽڵ�, ѡ�нڵ���Ҳ�����`��`�ַ�, �ٵ��`Start`��ť, �ͻ�����v2ray����. ����־�����л��ж�Ӧ��־���.  
�������������socks5����, ����ͨ��SwitchyOmega�������socks5����, ��ַΪ127.0.0.1, �˿�Ϊ�ղ���Settings����д�Ķ˿�, �Ϳ���ʹ����.

### ֹͣ
���`Stop`��ť, �ͻ�ֹͣ��ǰ��v2ray����. ���`Quit`��ť�˳�Ӧ��, Ҳ��ֹͣ��ǰ��v2ray����(������Ѿ�������v2ray).

## ж��
ж�ؿ���ʹ��dpkg����  
```shell
sudo dpkg --purge v2rock
```
�����Ҫ��������ļ�, ��Ҫɾ�� `~/.config/v2ray` Ŀ¼
```shell
rm -rf ~/.config/v2rock
```

## ��Դ�������
��װ����  
```shell
sudo apt install build-essential qt5-default libssl-dev qt5keychain-dev
```
Git����  
```shell
git clone https://github.com/IOsetting/v2rock.git
```
ִ�б���
```shell
cd v2rock
qmake && make -j4
```
��v2rock/v2rockĿ¼�»�����v2rock�������ļ�


