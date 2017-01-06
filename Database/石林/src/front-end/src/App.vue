<template>
  <div id="app">
    <navbar :menus="menus" :userName="userName"></navbar>
    <div id="content">
      <login v-if="showing === 'login'"></login>
      <dict v-if="showing === '字典管理'"></dict>
      <status v-if="showing === '总览'"></status>
      <user v-if="showing === '用户管理'"></user>
      <officer v-if="showing === '执法人员管理'"></officer>
      <news v-if="showing === '新闻策略管理'"></news>
      <div class="bottom">Copyright (c) 2016-2017 ntzyz. All rights reserved.</div>
    </div>
  </div>
</template>

<script>
import Navbar from './Navbar';
import Login from './Login';
import Dict from './Dict';
import Status from './Status';
import User from './User';
import Officer from './Officer';
import News from './News';

export default {
  name: 'app',
  components: { Navbar, Login, Dict, Status, User, Officer, News },
  data () {
    return {
      menus: ['总览', '字典管理', '用户管理', '执法人员管理', '新闻策略管理'],
      showing: 'login',
      accessToken: '',
      userName: '',
      autoLogin: false,
    }
  },
  methods: {
    tabNavigate: function(dest) {
      console.log('Navigating to: ', dest);
      let content = document.querySelector('#content');
      content.style.opacity = 0;
      setTimeout(() => {
        this.showing = dest >= 0 ? this.menus[dest] : 'login';
        content.style.opacity = 1;
      }, 100);
    }
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
}
.bottom {
  margin-top: 50px;
  text-align: center;
  color: gray;
}
#content {
  transition: all ease 0.1s;
}
ul.tabs {
  overflow: hidden;
}
ul.tabs > li {
  cursor: pointer;
}
ul.tabs > .indicator {
  background-color: #009688;
}
ul.tabs > li > a, ul.tabs > li > a:active {
  color: #009688!important; 
}
</style>