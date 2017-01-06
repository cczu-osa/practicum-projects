<template>
  <div id="login" style="opacity: 0;">
    <h2>需要授权</h2>
    <h5>请输入用户名和密码。</h5>
    <div class="row">
      <div class="input-field col s6">
        <input id="username" type="text" class="validate" name="username" v-model="username">
        <label for="username">用户名</label>
      </div>
    </div>
    <div class="row">
      <div class="input-field col s6">
        <input id="password" type="password" class="validate" name="password" v-model="password">
        <label for="password">密码</label>
      </div>
    </div>

    <div class="row" style="margin-bottom: 20px;">
      <div class="col s6">
        <input type="checkbox" class="filled-in" id="remember" />
        <label for="remember">记住登录</label>
      </div>
    </div>

    <div class="row">
      <a class="waves-effect waves-light btn" v-on:click="startLogin">登录</a>
      <a class="waves-effect waves-light btn" v-on:click="reset">重置</a>
    </div>
  </div>
</template>

<script>
export default {
  name: 'login',
  data () {
    return {
      username: 'root',
      password: '233666',
    }
  },
  methods: {
    startLogin: function () {
      this.$http.get(`/api/v1/portal?username=${this.username}&password=${this.password}`).then(
      (response) => {
        if (response.body.status === 'ok') {
          this.$parent.accessToken = response.body.token;
          this.$parent.userName = this.username;

          // Update cookie if need.
          let checker = document.querySelector('#remember');
          if (checker.value === 'on') {
            document.cookie = 'token=' + response.body.token;
          }

          this.$parent.tabNavigate(0);
        }
        else {
          Materialize.toast('登录失败，用户名或密码错误！', 4000);
        }
      })
    },
    reset: function () {
      this.username = this.password = "";
      setTimeout(() => { Materialize.updateTextFields(); }, 50);
    },
  },
  created: function() {
    this.reset();
    this.$nextTick(() => {
      let cookies = document.cookie.split('; ').filter(cookie => cookie.indexOf('token') === 0);
      if (cookies.length === 0) {
        document.querySelector('#login').style.opacity = 1;
        return;
      }
      else {
        let token = cookies[0].split('=')[1];
        this.$http.get('/api/v1/status?token=' + token).then((res) => {
          if (res.body.status === 'err') {
            document.querySelector('#login').style.opacity = 1;
          } 
          else {
            this.$parent.accessToken = token;
            this.$parent.userName = res.body.user;
            this.$parent.autoLogin = true;
            this.$parent.tabNavigate(0);
          }
        })
      }
    })
  }
}
</script>

<style scoped>
  #login {
    max-width: 800px;
    margin: 0 auto;
  }
  h2 {
    font-size: 22px;
  }
  h5 {
    font-size: 16px;
  }
  .row {
    margin-bottom: 0;
  }
</style>
