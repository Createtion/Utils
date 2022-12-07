# above install gh

1. conda install 
```python
bash Anaconda3-2022.05-Linux-x86_64.sh
```
> * yes to all
> * open .bashrc
```
# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/path/to/anaconda3/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/path/to/anaconda3/etc/profile.d/conda.sh" ]; then
        . "/path/to/anaconda3/etc/profile.d/conda.sh"
    else
        export PATH="/path/to/anaconda3/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<

```
2. gh install

```python
conda install gh   # input y 
gh auth login    # init auth
chose GitHub.com
HTTPS
yes
Paste an authentication token
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
gh repo clone Createtion/Projection
```

# git stash

> 1. 若有紧急bug需修复时，此时工作区还未提交，且此时工作尚未完成，无法提交，此时可使用 git stash ，将当前工作现场隐藏，等bug修复完成后在恢复现场。

> 2. 隐藏后使用 git status 查看工作区，此时工作区时干净的。修复完成后，使用 git stash list 可查看工作现场。

> 3. 使用 git stash apply 恢复现场，但恢复后， stash 内容 并没有删除，需要使用命令 git stash drop 删除。

> 4. 或者使用 git stash pop 命令，恢复现场同时删除stash内容。再用 git stash list 命令查看，就看不到任何stash内容了。

> 5. 可以多次使用 git stash 命令‘储藏’工作区，恢复时，先用 git stash list 查看，后可指定要恢复的stash，如 git stash apply 

# git large file storage

```python

Linux

    curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash

    sudo apt-get install git-lfs

    git lfs install
```

```python
git lfs track filename.{sh & gz & pth}
git add filename.{sh & gz & pth}
git push
```
# push
git config --global  --unset https.https://github.com.proxy 
git config --global  --unset http.https://github.com.proxy 

git config --global --unset http.proxy 
git config --global --unset https.proxy
