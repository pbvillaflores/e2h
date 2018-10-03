import speedtest,time
from datetime import datetime 

st = speedtest.Speedtest()
bs = st.get_best_server()
print('best server,',bs)
while True:  
  dl = st.download()
  ul = st.upload()
  with open(r'c:\users\paolo\st.csv','a') as f:
    f.write(str(datetime.now() )[:19]+',%0.0f,%0.0f\n'%(dl,ul) )
  time.sleep(300)
    
