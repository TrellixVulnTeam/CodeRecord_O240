#-*-coding:utf-8-*-
import urllib
import urllib2
import os
import json
import sys
import hashlib
import tablib

# h =hashlib.md5()
# h.update("http://webfotoalbums.nl/login.php?message_id=c83bd 5cf7325af222f2fda44ac49d864&message_icon=info")
# print h.hexdigest()
#api = 'fe1ce87df6bce7e0cc724da5d6d08fe461fd2fa25d63bbd0636120417c994db2'
#param = {'resource': md5, 'apikey': api, 'allinfo': '1'}
# md5=  '96dff36b5275c67e35097d77a120d0d4'
# sha1 ='ccb088c93654eab254412cafabac957dc758ba29'
# sha256 = "bd927d915f19a89468391133465b1f2fb78d7a58178867933c44411f4d5de8eb"
# print len(sha1)
# base = 'https://www.virustotal.com/vtapi/v2/'
api = '1a5e1ee7caabe91fd89d6f841055d6666be2625ee897b8c9e5c305d76abe72ad'

# 域名查询绑定过的IP
def domain_map_ip(domain_info):

	domain_url = 'https://www.virustotal.com/vtapi/v2/domain/report'
	parameters = {'domain': domain_info, 'apikey': api}
	response = urllib.urlopen('%s?%s' % (domain_url, urllib.urlencode(parameters))).read()
	ipaddr_list = json.loads(response)['resolutions']
	return ipaddr_list

#IP查询绑定过的域名
def ip_map_domain(ip_info):

	ip_url = 'https://www.virustotal.com/vtapi/v2/ip-address/report'
	parameters = {'ip': ip_info, 'apikey': api}
	response = urllib.urlopen('%s?%s' % (ip_url, urllib.urlencode(parameters))).read()
	domain_list = json.loads(response)['resolutions']
	return domain_list

# 导出XLS
def into_els(obj_list,filename):

	headers = (obj_list[0].keys())
	path = "D:\\0utCode\\"
	all_data = []
	for i in obj_list:
		datalist =i.values()
		all_data.append((datalist[0],datalist[1]))

	data_xls = tablib.Dataset(*all_data, headers=headers)
	open(path+filename+'.xls',"wb").write(data_xls.xls)


#读取域名文件函数
def domain_read_file(file_path):
    if not os.path.exists(file_path):
        print 'Please confirm correct filepath !'
        sys.exit(0)
    else:
        Domain_list = []
        with open(file_path, 'r') as source:
            for line in source:
                Domain_list.append(line.rstrip('\r\n').rstrip('\n'))
    for Domain_text in Domain_list:
        domain_list_result = domain_map_ip(Domain_text)
        print "## %s domain_list :\n" % Domain_text, domain_list_result
        into_els(domain_list_result, Domain_text)

#读取IP文件函数
def ip_read_file(file_path):
    if not os.path.exists(file_path):
        print 'Please confirm correct filepath !'
        sys.exit(0)
    else:
        ip_list = []
        with open(file_path, 'r') as source:
            for line in source:
                ip_list.append(line.rstrip('\r\n').rstrip('\n'))
    for ip_text in ip_list:
        ipaddr_list = ip_map_domain(ip_text)
        print "## %s ip_list :\n"%ip_text,ipaddr_list
        into_els(ipaddr_list,ip_text)



if __name__ == '__main__':
    domain_file_str=raw_input('Input file domain.txt filepath eg:D:\\\\domain.txt \n')
    ip_file_str=raw_input('Input file ip.txt filepath eg:D:\\\\ip.txt \n')
    domain_read_file(domain_file_str)    #读取域名文件
    ip_read_file(ip_file_str)    #读取IP文件