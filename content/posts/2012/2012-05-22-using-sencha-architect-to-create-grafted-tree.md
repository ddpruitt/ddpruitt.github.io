---
title: 'Using Sencha Architect to Create Grafted Tree'
date: 2012-05-22T05:54:00.000-07:00
draft: false
url: 
tags: 
- ExtJS
---

Clint Harris posted [How To Use ExtJS 4 TreePanels with Both Static Data and Model Stores](http://www.clintharris.net/2011/how-to-use-extjs-4-treepanels-with-both-static-data-and-model-stores/ "How To Use ExtJS 4 TreePanels with Both
      Static Data and Model Stores"). He has a great explanation of how to "graft" branches onto an ExtJS TreeView. In his example he is creating an Admin tree form that gives him access to Settings and Users.  
  
I recreated his project using [Sencha Architect 2](http://www.sencha.com/products/architect/ "Sencha Architect 2").  
  
The trickiest part was getting the tree to load the data correctly. I used the **onLaunch** function of the controller to get access to the TreeStores in order to call the userStore.setRootNode() and then to append the userStore to the settingsTreeStore.  
  
Notice how I am using **this.getUserTreeStoreStore()** and **this.getSettingTreeStoreStore()**. Yes, I had to use "StoreStore" because I named my stores UserTreeStore and SettingTreeStore, then the Controller tacks on the second "Store" when it creates the get function.  
  
```javascript  
Ext.define('GraftedTreeApp.controller.TreePanelController', {  
      extend: 'Ext.app.Controller',  
      
      models: [  
            'UserModel'  
      ],  
      stores: [  
            'SettingTreeStore',  
            'UserTreeStore'  
      ],  
      
      onLaunch: function() {    
            var userStore = this.getUserTreeStoreStore();  
            userStore.setRootNode({  
            text: 'Users',  
            leaf: false,  
            expanded: false  
      });  
      
      // Graft our userTreeStore into the settingsTreeStore. Note that the call  
      // to .expand() is what triggers the userTreeStore to load its data.  
      var settingsTreeStore = this.getSettingTreeStoreStore();  
      settingsTreeStore.getRootNode().appendChild(userStore.getRootNode()).expand();  
      }  
  
});  
  
```  
  
Pretty much everything else was setting properties in the Sencha Architect UI. The project files are [here](http://techshorts.ddpruitt.net/wp-content/uploads/2012/05/graftedTree.zip).  
  
**NOTE:** There is a known bug in this solution, when you collapse and expand the User node it loads more User data, after which the User node cannot be collapsed.