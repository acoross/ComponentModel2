using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Dash : MonoBehaviour {
    public GameObject movePrefab;
    public float speed = 10f;
    public int effect_count = 5;
    public float actionTime = 0.1f;

    bool moving = false;
    float moveAccumTime = 0f;
    Vector2 movingDirection = Vector2.zero;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void Run() {
        StartCoroutine("DashCoroutine");
    }

    IEnumerator DashCoroutine() {
        if (moving) {
            yield break;
        }

        moving = true;
        movingDirection = transform.TransformVector(Vector3.right);

        //var sock = GetComponent<MySocket>();
        //sock.Dash(movingDirection, speed, actionTime, effect_count);

        int count = 0;
        while (count < effect_count)
        {
            moveAccumTime += Time.deltaTime;
            if (moveAccumTime > actionTime)
            {
                var moveEffect = GameObject.Instantiate(movePrefab);
                moveEffect.transform.position = transform.position;

                moveAccumTime -= actionTime;
                count++;
            }

            transform.Translate(movingDirection * speed * Time.deltaTime, Space.World);
            yield return null;
        }

        DashEnd();
    }

    public void DashEnd()
    {
        moveAccumTime = 0f;
        moving = false;
    }
}
