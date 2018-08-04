sudo apt-get install ssh ntp -y
sudo apt-get update -y
sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils -y
sudo apt-get install libboost-all-dev -y
sudo apt-get install git -y
sudo apt-get install software-properties-common -y
sudo add-apt-repository ppa:bitcoin/bitcoin -y
sudo apt-get update -y


sudo aws s3api put-bucket-cors --bucket shinhj0728 --cors-configuration '{
       "CORSRules": [{
         "AllowedHeaders": ["*"],
         "AllowedMethods": ["GET"],
         "AllowedOrigins": ["http://52.78.105.123:8080"]
       }]
     }'

sudo aws s3api put-bucket-cors --bucket shinhj0728 --cors-configuration '{
       "CORSRules": [{
         "AllowedHeaders": ["*"],
         "AllowedMethods": ["GET"],
         "AllowedOrigins": ["http://ec2-52-78-105-123.ap-northeast-2.compute.amazonaws.com:8080"]
       }]
     }'

sudo aws s3 cp skorea_provinces_geo_simple.json s3://shinhj0728/

aws s3api put-object-acl --bucket shinhj0728 --key skorea_provinces_geo_simple.json --acl public-read


sudo lsof -n -i :8080 | grep LISTEN


.es(index=exam, timefield=date, metric='avg:score' ).label('평균'),
.es(index=exam, timefield=date, metric='avg:score', offset=-1M ).label('지난달평균')



ssh -i shin.pem ec2-user@ec2-52-78-105-123.ap-northeast-2.compute.amazonaws.com
