<template>
  <div id="dict">
    <h2>字典管理</h2>
    <div id="dictMain">
      <div v-if="showing === 'list'">
        <h5>对字典进行管理操作。</h5>
        <div class="row">
          <!-- table -->
          <table class="highlight">
            <thead>
              <tr>
                <th>字典编号</th>
                <th>字典类型</th>
                <th>字典内容</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="(dict, index) in dictArray" v-on:click="dictEdit(index)">
                <td>{{ dict.id }}</td>
                <td>{{ dict.category }}</td>
                <td>{{ dict.value }}</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="row">
          <!-- button -->
          <a class="waves-effect waves-light btn" v-on:click="dictAdd">添加</a>
        </div>
      </div>

      <div class="row" v-if="showing === 'edit'">
        <h5>编辑/创建字典</h5>
        <!-- form -->
        <div class="row frm">
          <div class="input-field col s6">
            <input id="dict_id" type="text" class="validate" v-model="editing.id" disabled>
            <label for="dict_id">字典编号</label>
          </div>
        </div>

        <div class="row frm">
          <div class="input-field col s6">
            <select id="categorySelector">
              <option value="" disabled selected>Choose your option</option>
              <option v-for="(cat, index) in categoryList" :value="index"> {{ cat }} </option>
            </select>
            <label>字典类型</label>
          </div>
        </div>

        <div class="row frm">
          <div class="input-field col s6">
            <input id="dict_value" type="text" class="validate" v-model="editing.value">
            <label for="dict_value">字典内容</label>
          </div>
        </div>

        <div class="row frm">
          <!-- button -->
          <div class="input-field inline col s6">
            <a class="waves-effect waves-light btn" v-on:click="back">返回</a>
            <a class="waves-effect waves-light btn" v-on:click="dictSave">保存</a>
            <a class="waves-effect waves-light btn red right" v-on:click="dictDelete" v-if="this.editing.id !== ''">删除</a>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import common from './common.js';

export default {
  name: 'dict',
  data () {
    return {
      dictArray: [],
      showing: 'list',
      categoryList: [],
      editing: {
        id: '',
        category: '',
        value: '',
      }
    }
  },
  methods: {
    dictAdd: function () {
      this.switchto('edit', () => {
        document.querySelector('select').value = common.CATEGORY[this.editing.category];
      });
    },
    dictEdit: function (index) {
      this.editing = JSON.parse(JSON.stringify(this.dictArray[index]));
      
      this.switchto('edit', () => {
        document.querySelector('select').value = common.CATEGORY[this.editing.category];
      });
    },
    dictSave: function () {
      this.editing.id = encodeURIComponent(this.editing.id);
      this.editing.category = encodeURIComponent(common.categoryToString(document.querySelector('select').value));
      this.editing.value = encodeURIComponent(this.editing.value);

      let finishHandler = (data) => {
        if (data.body.status === 'ok') {
          this.refresh();
          this.back();
        }
        else {
          Materialize.toast('权限不足，只有管理员和执法人员可以修改和创建字典', 4000)
        }
      }

      if (this.editing.id !== '') {
        this.$http.post(`/api/v1/dict?token=${this.$parent.accessToken}&id=${this.editing.id}&category=${this.editing.category}&value=${this.editing.value}`).then(finishHandler)
      }
      else {
        this.$http.put(`/api/v1/dict?token=${this.$parent.accessToken}&category=${this.editing.category}&value=${this.editing.value}`).then(finishHandler);
      }
    },
    dictDelete: function () {
      this.editing.id = encodeURIComponent(this.editing.id);
      if (this.editing.id !== '') {
        this.$http.delete(`/api/v1/dict?token=${this.$parent.accessToken}&id=${this.editing.id}`).then((res) => {
          console.log(res);
          if (res.body.status === 'ok') {
            this.refresh();
            this.back();
          }
          else {
            Materialize.toast('权限不足，只有管理员和执法人员可以删除字典', 4000)
          }
        })
      }
    },
    back: function () {
      this.editing.id = '';
      this.editing.category = this.editing.value = "";
      this.switchto('list');
    },
    switchto: function (dest, callback) {
      let content = document.querySelector('#dictMain');
      content.style.opacity = 0;
      setTimeout(() => {
        this.showing = dest;
        if (callback) setTimeout(() => {callback();}, 0);
        content.style.opacity = 1;
      }, 100);
    },
    refresh: function() {
      this.$http.get(`/api/v1/dict?token=${this.$parent.accessToken}`).then((response) => {
        this.dictArray = response.body.dataset;
      })
    }
  },
  created: function () {
    let accessToken = this.$parent.accessToken;
    if (!accessToken || accessToken === '')
      this.$parent.tabNavigate(-1);

    for (let key in common.CATEGORY) {
      this.categoryList.push(key);
    }

    this.refresh();
  },
  watch: {
    showing: function(val) {
      setTimeout(() => {
        $('select').material_select();
        Materialize.updateTextFields(); 
      }, 0);
    }
  }
}
</script>

<style scoped>
  #dict {
    max-width: 800px;
    margin: 0 auto;
  }
  #dictMain {
    transition: all ease 0.1s;
  }
  h2 {
    font-size: 22px;
  }
  h5 {
    font-size: 16px;
  }
  div.frm {
    margin-bottom: 0;
  }
</style>
