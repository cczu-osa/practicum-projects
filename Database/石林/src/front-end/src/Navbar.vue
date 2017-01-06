<template>
  <div class="navbar-fixed">
    <nav>
      <div class="nav-wrapper teal">
        <a class="brand-logo left" style="padding-left: 20px;">
          <span style="cursor: pointer" v-on:click="$parent.tabNavigate(0)">XX 管理系统</span>
          <span style="font-size: 14px;" v-if="userName !== ''">
            当前登录：{{ userName }}
          </span>
        </a>
        <ul class="right hide-on-med-and-down">
          <li v-for="(item, index) in menus">
            <a v-on:click="$parent.tabNavigate(index)">{{ item }}</a>
          </li>
          <li><a v-on:click="logout">退出系统</a></li>
        </ul>
      </div>
    </nav>
  </div>
</template>

<script>
export default {
  name: 'navbar',
  props: ['menus', 'userName'],
  data () {
    return {
      submenus: [],
    }
  },
  methods: {
    logout: function() {
      document.cookie.split(";").forEach(function(c) { document.cookie = c.replace(/^ +/, "").replace(/=.*/, "=;expires=" + new Date().toUTCString() + ";path=/"); });
      window.location = window.location;
    }
  },
  created: function() {
    this.submenus = this.menus;
  }
}
</script>

